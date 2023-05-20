#pragma once

#include "Module.h"
#include "ModuleFiles.h"
#include "Texture.h"
#include "File_Model.h"
#include "FfmpegVideoPlayer.h"
#include "UUID.h"
#include "glew.h"

class GameObject;
class FileTree;

class Resource
{
public:
    Resource() {}
    virtual ~Resource() {}

    void Dereference()
    {
        referenceCount--;
        if (referenceCount == 0)
            UnLoad();
    }

    /// To be called when the Asset corresponding this Resource gets deleted.
    virtual void Destroy() {}

    virtual void ReImport(const std::string& filePath) {}

    ResourceType _type = ResourceType::UNDEFINED;

protected:
    virtual void UnLoad() {}

public:
    ResourceType type = ResourceType::UNDEFINED;
    uint referenceCount = 0;
    std::string resourcePath = "";
    std::string debugName = "NoName";
    uint UID = 0;
};

class ResourceTexture : public Resource
{
public:
    ResourceTexture() { type = ResourceType::TEXTURE; }

    ~ResourceTexture() {}

private:
	void UnLoad() override
	{
		if (OpenGLID == 0)
			return;

		glDeleteTextures(1, &OpenGLID);
		name = "";
		OpenGLID = 0;
		width = 0;
		height = 0;
		isTransparent = false;
	}

public:
    void Destroy() override;
    void ReImport(const std::string& filePath) override;
public:
    std::string name = "";
    uint OpenGLID = 0;
    int width = 0;
    int height = 0;
    bool isTransparent = false;
};

class ResourceModel : public Resource
{
public:
    ResourceModel() { type = ResourceType::MODEL; };

    ~ResourceModel() {};

    void CreateResourceMeshes();
    void UpdateResourceMeshes();

    ModelNode modelInfo;

    std::vector<ResourceMesh*> modelMeshes;

public:
    void Destroy() override;
    void ReImport(const std::string& filePath) override;

private:
    void CreateResourceMeshesRecursive(ModelNode& node);
    void UpdateResourceMeshesRecursive(ModelNode& node);

    uint currentLoadedMesh = 0;
};

class ResourceMesh : public Resource
{
public:
    ResourceMesh() { type = ResourceType::MESH; };
    ~ResourceMesh() {};

    void UnLoad() override
    {
        meshInfo.Clear();
        vertexNormals.clear();
        faceNormals.clear();

		if (VAO == 0)
			return;

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);

        glDeleteVertexArrays(1, &VertexNormalsVAO);
        glDeleteBuffers(1, &VertexNormalsVBO);
        glDeleteVertexArrays(1, &FaceNormalsVAO);
        glDeleteBuffers(1, &FaceNormalsVBO);

        VAO = 0;
        VBO = 0;
        IBO = 0;
        VertexNormalsVAO = 0;
        VertexNormalsVBO = 0;
        FaceNormalsVAO = 0;
        FaceNormalsVBO = 0;
    }

    void CreateBuffers();
    void CalculateNormalsAndAABB();

public:
    // Just for InputGeom use
    MeshInfo GetMeshInfo();

    void Destroy() override;
    void ReImport(const std::string& filePath) override;

    MeshInfo meshInfo;
    uint modelUID = 0;
    uint indexInsideModel = 0;

    std::vector<float3> vertexNormals;
    std::vector<float3> faceNormals;
    AABB localAABB;

    uint VertexNormalsVAO = 0;
    uint VertexNormalsVBO = 0;
    uint FaceNormalsVAO = 0;
    uint FaceNormalsVBO = 0;

    uint VAO = 0;
    uint VBO = 0;
    uint IBO = 0;
};

class ResourceAnimation : public Resource
{
public:
	ResourceAnimation() { type = ResourceType::ANIMATION; };
	~ResourceAnimation() {};

	void UnLoad() override
	{
		animation.Clear();
	}

public:
	void Destroy() override;
	void ReImport(const std::string& filePath) override;

	Animation3D animation;
	uint animUID = 0;


};

class ResourceShader : public Resource
{
public:
    ResourceShader() {};

    void Recompile(std::string text)
    {
        ModuleFiles::S_Save(resourcePath, text.data(), text.length(), false);
        ModuleFiles::S_Save(assetsPath, text.data(), text.length(), false);

        shader.Recompile(resourcePath);

        version = HelloUUID::GenerateUUID();
    }

    void UnLoad() override
    {
        shader.Clear();
    }

    void ReImport(const std::string& filePath) override;

    Shader shader;

    int version;

    bool _onEditor = false;
    std::string assetsPath = "";
};

class ResourceMaterial : public Resource
{
public:
    ResourceMaterial() {};
    ~ResourceMaterial() {};

    void UnLoad() override
    {
        // TODO: Comentar si esto deberiamos seguir haciendolo al descargar el material?
        //Save();
        material.Clear();
    }

    void ReImport(const std::string& filePath) override;

    void Save();

    Material material;
    std::string assetsPath = "";
};

class ResourceScript : public Resource
{
public:
    ResourceScript() {};

    void Destroy() override;

    std::string className = "";
};

class ResourcePrefab : public Resource
{
public:
    ResourcePrefab() { _type = ResourceType::PREFAB; };

    std::string path;
};

class ResourceVideo : public Resource
{
public:
    ResourceVideo() {};

    void UnLoad() override
    {
        if (video == nullptr)
            return;
        video->CleanUp();
    }

    FfmpegVideoPlayer* video = nullptr;
};

class ModuleResourceManager : public Module
{
public:
    ModuleResourceManager();

    ~ModuleResourceManager();

    bool Init() override;

    bool Start() override;

    /// <summary>
    /// Import a file from assets to our desired file format.
    /// </summary>
    static void S_ImportFile(const std::string& filePath);

    static void S_ReImportFile(const std::string& filePath, ResourceType resourceType);

    static Resource* S_LoadFile(const std::string& filePath);

    static void S_LoadFileIntoResource(Resource* resource);

    static bool S_IsFileLoaded(const std::string& fileName);

    static bool S_GetFileTree(FileTree*& tree);

    static std::string S_GetCurrentWorkingDir();

    static void S_UpdateFileTree();

    static void S_SerializeScene(GameObject*& g);
    /// Save scene, to a given path, but not saving it as the current scene inside config.xml.
    static void S_SerializeScene(GameObject*& g, const std::string& path);

    static bool S_DeserializeScene(const std::string& filePath);

    // Ruben Ayora
    static void S_SerializeToPrefab(GameObject* g, const std::string& folderPath);

    static GameObject* S_DeserializeFromPrefab(const std::string& filePath, GameObject* parent, bool loadingScene = false);
    static void S_DeserializePrefabsScripts(const std::string& filePath, std::vector<std::pair<GameObject*, uint>>& tempPrefab);

    static uint S_GetPrefabUID(const std::string& filePath);

    static void S_OverridePrefab(GameObject* g, const std::string& filePath, uint prefabUID);
	
	static void S_CreateResourceAnimation(const std::string& filePath, uint UID, const std::string& name, bool load = true);

	static void S_CreateResourceText(const std::string& filePath, uint UID, const std::string& name, bool load = true);
    /// <summary>
    /// Delete meta file and the resources attached to it. If you want to only destroy the resources, mark bool as true.
    /// </summary>
    static void S_DeleteMetaFile(const std::string& file, bool onlyResources = false, bool isReimporting = false);

    static void S_CreateResource(const MetaFile& metaFile);

    // Only for internal engine usage!
    static ResourceMesh* S_CreateResourceMesh(const std::string& filePath, uint UID, const std::string& name, bool load = true, ResourceModel* model = nullptr);

    static ResourceShader* S_CreateResourceShader(const std::string& filePath, uint UID, const std::string& name, bool load = true);

    static Resource* S_LoadResource(const uint& UID);

    static bool S_IsResourceCreated(const uint& UID);

    static std::vector<Resource*> S_GetResourcePool(ResourceType type);

private:
    static void GetResourcePath(ModelNode& node, std::vector<std::string>& vector);

    static void SerializeSceneRecursive(const GameObject* g, json& j);

    // Ruben Ayora
    static uint SerializeToPrefab(const GameObject* g, json& j, uint prefabUID = 0, uint parentUID = 0);

public:
    static std::map<std::string, Resource*> loadedResources;

    static std::map<uint, Resource*> resources;
    static FileTree* _fileTree;
    
};