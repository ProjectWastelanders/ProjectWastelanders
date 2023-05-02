#include "Headers.h"
#include "ModuleResourceManager.h"
#include "ModuleFiles.h"
#include "Console.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "json.hpp"
#include "FileTree.hpp"
#include "ModuleCommand.h"
#include "ScriptComponent.h"
#include "MaterialComponent.h"
#include "TextureComponent.h"

#include "GameObject.h"
#include "ModuleLayers.h"
#include "ModuleWindow.h"
#include "RenderManager.h"
#include "ModuleRenderer3D.h"
#include "Component.h"
#include "LayerEditor.h"
#include "Lighting.h"

// In create resource mesh method save my index and model UID.
// Save ResourceModel UID and index.
// On Deserialization, look for ModelUID. If exists, check if index exists. If exists, load that resource mesh.

std::map<std::string, Resource*> ModuleResourceManager::loadedResources;
std::map<uint, Resource*> ModuleResourceManager::resources;
FileTree* ModuleResourceManager::_fileTree = nullptr;

ModuleResourceManager::ModuleResourceManager()
{
	Console::S_Log("Initialaizing DevIL.");

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		Console::S_Log("Wrong DevIL version detected.");
	}
}

ModuleResourceManager::~ModuleResourceManager()
{
	for (auto& resource1 : resources)
	{
		RELEASE(resource1.second);
	}
	resources.clear();

	RELEASE(_fileTree);
}

bool ModuleResourceManager::Init()
{
	// Create checkers texture resource
	S_CreateResourceText("Null", CHECKERS_RESOURCE_UID, "Checkers", false);
	return true;
}

bool ModuleResourceManager::Start()
{
	_fileTree = new FileTree();

	ModuleFiles::S_UpdateFileTree(_fileTree);
	return true;
}

void ModuleResourceManager::S_ImportFile(const std::string& filePath)
{
	ResourceType type = ModuleFiles::S_GetResourceType(filePath);

	if (type == ResourceType::UNDEFINED)
	{
		Console::S_Log("Tried to import an undefined file. Filename: " + filePath);
		return;
	}

	// TODO: Restruct Inport parameters
	switch (type)
	{
	case ResourceType::MODEL:
	{
		std::string path = MeshImporter::ImportModel(filePath);
		ModuleFiles::S_CreateMetaData(filePath, path);
	}
	break;
	case ResourceType::ANIMATION:
	{
	}
	break;
	case ResourceType::TEXTURE:
	{
		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);
		std::string path = TextureImporter::ImportImage(ModuleFiles::S_GetFileName(filePath, false), buffer, size);
		ModuleFiles::S_CreateMetaData(filePath, path);
		RELEASE_ARRAY(buffer);
	}
	break;
	case ResourceType::HSCRIPT:
	case ResourceType::CPPSCRIPT:
	{
		ModuleFiles::S_CreateMetaData(filePath, "don't have resource path now");

		break;
	}
	case ResourceType::PREFAB:
	{
		ModuleFiles::S_CreateMetaData(filePath, filePath, S_GetPrefabUID(filePath));

		break;
	}
	case ResourceType::MATERIAL:
	{
		uint UUID = HelloUUID::GenerateGUID(filePath);
		std::string resourcePath = "Resources/Materials/" + std::to_string(UUID) + ".material";

		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);

		//Resources
		ModuleFiles::S_Save(resourcePath, &buffer[0], size, false);

		//Create Metadata
		ModuleFiles::S_CreateMetaData(filePath, resourcePath, UUID);
		RELEASE(buffer);
		break;
	}
	case ResourceType::SHADER:
	{
		uint UUID = HelloUUID::GenerateGUID(filePath);
		std::string resourcePath = "Resources/Shaders/" + std::to_string(UUID) + ".shader";

		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);

		//Resources
		ModuleFiles::S_Save(resourcePath, &buffer[0], size, false);

		//Create Metadata
		ModuleFiles::S_CreateMetaData(filePath, resourcePath, UUID);
		RELEASE(buffer);
		break;
	}
	case ResourceType::VIDEO:
	{
		// Temporal import method: Copy and paste file into Resources.
		uint UUID = HelloUUID::GenerateGUID(filePath);

		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);
		std::string resourcePath = "Resources/Videos/" + std::to_string(UUID) + ".HVideo";

		ModuleFiles::S_Save(resourcePath, &buffer[0], size, false);
		ModuleFiles::S_CreateMetaData(filePath, resourcePath, UUID);
		RELEASE(buffer);
	}
	break;
	default:
		break;
	}
}

void ModuleResourceManager::S_ReImportFile(const std::string& filePath, ResourceType resourceType)
{
	char* buffer = nullptr;
	uint size = 0;
	MetaFile meta;

	if (resourceType != ResourceType::UNDEFINED)
	{	
		size = ModuleFiles::S_Load(filePath, &buffer);
		meta = ModuleFiles::S_LoadMeta(filePath + ".helloMeta");
	}

	// TODO: Restruct Inport parameters
	switch (resourceType)
	{
	case ResourceType::MODEL:
	{
		std::string path = MeshImporter::ImportModel(filePath);
		ModuleFiles::S_UpdateMetaData(filePath, path);
		if (resources[meta.UID] != nullptr)
		{
			resources[meta.UID]->ReImport(path);
			S_SerializeScene(ModuleLayers::rootGameObject);
			std::string savePath = Application::Instance()->xml->GetConfigXML().FindChildBreadth("currentScene").node.attribute("value").as_string();
			ModuleLayers::S_RequestLoadScene(savePath);
		}
	}
	break;
	case ResourceType::TEXTURE:
	{
		std::string path = TextureImporter::ImportImage(ModuleFiles::S_GetFileName(filePath, false), buffer, size);
		ModuleFiles::S_UpdateMetaData(filePath, path);
		if (resources[meta.UID] != nullptr)
		{
			resources[meta.UID]->ReImport(path);
		}
	}
	break;
	case ResourceType::HSCRIPT:
	case ResourceType::CPPSCRIPT:
	{
		// Compile DLL and call Hot reload
		LayerGame::S_RequestDLLCompile();
		LayerGame::S_RequestHotReload();
		ModuleFiles::S_UpdateMetaData(filePath, "null");
	}
	break;
	case ResourceType::MATERIAL:
	{
		if (resources[meta.UID] != nullptr && resources[meta.UID]->referenceCount != 0)
		{
			ModuleFiles::S_UpdateMetaData(filePath, meta.resourcePath); // We do this before reimporting, because the new resource file will be named like the old, and this destroys that file.
			resources[meta.UID]->ReImport(""); // No necessary path for this kind of ReImport
		}
		break;
	}
	case ResourceType::SHADER:
	{
		if (resources[meta.UID] != nullptr)
		{
			ModuleFiles::S_UpdateMetaData(filePath, meta.resourcePath); // We do this before reimporting, because the new resource file will be named like the old, and this destroys that file.
			resources[meta.UID]->ReImport(""); // No necessary path for this kind of ReImport
		}
		break;
	}
	case ResourceType::PREFAB:
	{
		if (resources[meta.UID] != nullptr)
		{
			ModuleFiles::S_UpdateMetaData(filePath, meta.resourcePath); 
		}
	}
	break;
	case ResourceType::ANIMATION:
	{
		if (resources[meta.UID] != nullptr)
		{
			ModuleFiles::S_UpdateMetaData(filePath, meta.resourcePath);
		}
	}
	break;
	}

	RELEASE_ARRAY(buffer);
}

void ModuleResourceManager::S_LoadFileIntoResource(Resource* resource)
{
	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(resource->resourcePath, &buffer);

	if (size == 0)
		return;

	switch (resource->type)
	{
	case ResourceType::MODEL:
	{
		// When loading a Model, we need to load each mesh of the model into separate Mesh Resources.
		// A meta file of a model IS NOT a ResourceMesh instance.
		// When loading a Model, all Resources of that model get loaded, but there is no ResourceModel.
		ResourceModel* modelRes = (ResourceModel*)resource;
		MeshImporter::LoadModelIntoResource(modelRes);
	}
	break;
	case ResourceType::TEXTURE:
	{
		ResourceTexture* textureRes = (ResourceTexture*)resource;
		TextureImporter::Load(buffer, size, textureRes);
	}
	break;
	case ResourceType::MESH:
	{
		ResourceMesh* meshRes = (ResourceMesh*)resource;
		meshRes->meshInfo.LoadFromBinaryFile(meshRes->resourcePath);
		meshRes->CreateBuffers();
		meshRes->CalculateNormalsAndAABB();
	}
	break;
	case ResourceType::ANIMATION:
	{
		ResourceAnimation* animRes = (ResourceAnimation*)resource;
		animRes->animation.LoadFromBinaryFile(animRes->resourcePath);

	}
	break;
	case ResourceType::SHADER:
	{
		ResourceShader* shaderRes = (ResourceShader*)resource;
		shaderRes->shader = Shader(shaderRes->resourcePath);
		shaderRes->version = HelloUUID::GenerateUUID();
	}
	break;
	case ResourceType::MATERIAL:
	{
		ResourceMaterial* materialRes = (ResourceMaterial*)resource;
		materialRes->material.LoadJSON(materialRes->assetsPath);
	}
	break;
	case ResourceType::VIDEO:
	{
		ResourceVideo* videoRes = (ResourceVideo*)resource;
		videoRes->video = new FfmpegVideoPlayer(videoRes->resourcePath.c_str());
	}
	break;
	}

	RELEASE_ARRAY(buffer);
}

Resource* ModuleResourceManager::S_LoadFile(const std::string& filePath)
{
	ResourceType type = ModuleFiles::S_GetResourceType(filePath);

	if (type == ResourceType::UNDEFINED)
	{
		Console::S_Log("Tried to load an undefined file. Filename: " + filePath);
		return nullptr;
	}

	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(filePath, &buffer);

	switch (type)
	{
	case ResourceType::MESH:
	{
		/*ResourceMesh* resource = new ResourceMesh();
		resource->meshParent = MeshImporter::LoadMesh(filePath);
		RELEASE_ARRAY(buffer);
		loadedResources[ModuleFiles::S_GetFileName(filePath, true)] = resource;
		return resource;*/
	}
	break;
	case ResourceType::TEXTURE:
	{
		// If we already loaded this texture, return its pointer.
		if (loadedResources.find(ModuleFiles::S_GetFileName(filePath, true)) != loadedResources.end())
		{
			RELEASE_ARRAY(buffer);
			return loadedResources[ModuleFiles::S_GetFileName(filePath, true)];
		}

		ResourceTexture* resource = new ResourceTexture();
		TextureImporter::Load(buffer, size, resource);
		RELEASE_ARRAY(buffer);
		loadedResources[ModuleFiles::S_GetFileName(filePath, true)] = resource;
		return resource;
	}
	}

	RELEASE_ARRAY(buffer);

	return nullptr;
}

bool ModuleResourceManager::S_IsFileLoaded(const std::string& fileName)
{
	return loadedResources.find(fileName) != loadedResources.end();
}

bool ModuleResourceManager::S_GetFileTree(FileTree*& tree)
{
	if (_fileTree)
	{
		tree = _fileTree;
		return true;
	}
	return false;
}

std::string ModuleResourceManager::S_GetCurrentWorkingDir()
{
	return _fileTree->_currentDir->path;
}

void ModuleResourceManager::S_UpdateFileTree()
{
	ModuleFiles::S_UpdateFileTree(_fileTree);
}

void ModuleResourceManager::S_SerializeScene(GameObject*& g)
{
	if (!g)
		return;

	// Create json
	json j;

	// Write json
	SerializeSceneRecursive(g, j);

	// Save json 
	std::string savePath = Application::Instance()->xml->GetConfigXML().FindChildBreadth("currentScene").node.attribute("value").as_string();

	if (savePath == "null")
		savePath = _fileTree->_currentDir->path + g->name + ".HScene";

	// Change Title
	std::string scenePath = " -- CurrentScene: " + savePath;

	ModuleWindow::S_AddTitleExtraInfo(scenePath);

	std::string buffer = j.dump(4);

	ModuleFiles::S_Save(savePath, &buffer[0], buffer.size(), false);
}

// Ruben Ayora
void ModuleResourceManager::S_SerializeToPrefab(GameObject* g, const std::string& folderPath)
{
	if (!g)
		return;

	std::string prefabPath = folderPath + "/" + g->GetName() + ".HPrefab";

	uint prefabUID = S_GetPrefabUID(prefabPath);

	// Create json
	json j;
	// Write json
	g->_prefabUID = SerializeToPrefab(g, j, prefabUID);

	g->SetAllChildsPrefabUID(g->_prefabUID);

	std::string buffer = j.dump();

	ModuleFiles::S_Save(prefabPath, &buffer[0], buffer.size(), false);
}

void ModuleResourceManager::S_SerializeScene(GameObject*& g, const std::string& path)
{
	if (!g)
		return;
	// Create json
	json j;
	// Write json
	SerializeSceneRecursive(g, j);

	std::string buffer = j.dump(4);

	ModuleFiles::S_Save(path, &buffer[0], buffer.size(), false);
}

// Ruben Ayora
GameObject* ModuleResourceManager::S_DeserializeFromPrefab(const std::string& filePath, GameObject* parent, bool loadingScene)
{
	char* buffer = nullptr;

	uint size = ModuleFiles::S_Load(filePath, &buffer);

	if (size == 0)
		return nullptr;

	json sceneFile = json::parse(buffer);
	RELEASE(buffer);

	std::vector<std::pair<GameObject*, uint>> temp;

	// First  create game objects
	for (int i = 0; i < sceneFile.size(); i++)
	{
		GameObject* g = new GameObject(nullptr, sceneFile[i]["Name"], sceneFile[i]["Tag"], sceneFile[i]["UID"]);
		g->SetPrefabUID(sceneFile[i]["PrefabUID"]);
		temp.push_back(std::make_pair(g, sceneFile[i]["ParentUID"]));
	}

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].second != 0 && ModuleLayers::gameObjects.count(temp[i].second) != 0)
			temp[i].first->SetParent(ModuleLayers::gameObjects[temp[i].second]);
		else if (!loadingScene)
			temp[i].first->SetParent(parent);
	}

	// then add their components
	// TODO: Script components whould be the last to get serialied, because they may need to reference another components instances.
	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];
			if (componentType == Component::Type::SCRIPT || componentType == Component::Type::MATERIAL)
				continue;
			temp[i].first->AddComponentSerialized(componentType, object[j]);
		}
	}

	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];
			if (componentType != Component::Type::MATERIAL)
				continue;
			temp[i].first->AddComponentSerialized(componentType, object[j]);
		}
	}

	if (!loadingScene)
	{
		for (int i = 0; i < sceneFile.size(); i++)
		{
			// Create components
			json object = sceneFile[i]["Components"];
			for (int j = 0; j < object.size(); j++)
			{
				Component::Type componentType = object[j]["Type"];
				if (componentType != Component::Type::SCRIPT)
					continue;
				temp[i].first->AddComponentSerialized(componentType, object[j]);
			}
		}
	}

	for (int i = 0; i < sceneFile.size(); i++)
	{
		temp[i].first->SetActive(sceneFile[i]["Active"]);
	}

	S_OverridePrefab(temp[0].first, filePath, sceneFile[0]["PrefabUID"]);

	return temp[0].first;
}

void ModuleResourceManager::S_DeserializePrefabsScripts(const std::string& filePath, std::vector<std::pair<GameObject*, uint>>& tempPrefab)
{
	char* buffer = nullptr;

	uint size = ModuleFiles::S_Load(filePath, &buffer);

	if (size == 0)
		return;

	json sceneFile = json::parse(buffer);
	RELEASE(buffer);

	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];
			if (componentType != Component::Type::SCRIPT)
				continue;
			tempPrefab[i].first->AddComponentSerialized(componentType, object[j]);
		}
	}
}

uint ModuleResourceManager::S_GetPrefabUID(const std::string& filePath)
{
	char* buffer = nullptr;

	uint size = ModuleFiles::S_Load(filePath, &buffer);

	if (size == 0)
		return 0;

	json sceneFile = json::parse(buffer);
	RELEASE(buffer);

	return sceneFile[0]["PrefabUID"];
}

void ModuleResourceManager::S_OverridePrefab(GameObject* g, const std::string& filePath, uint prefabUID)
{
	if (!g)
		return;
	// Create json
	json j;
	// Write json
	g->_prefabUID = SerializeToPrefab(g, j, prefabUID);

	std::string buffer = j.dump();

	std::string prefabPath = filePath;

	ModuleFiles::S_Save(prefabPath, &buffer[0], buffer.size(), false);
}

bool ModuleResourceManager::S_DeserializeScene(const std::string& filePath)
{
	// Deselect current selected game object
	LayerEditor::S_SetSelectGameObject(nullptr);
	Application::Instance()->renderer3D->renderManager.RemoveSelectedMesh();
	
	char* buffer = nullptr;

	uint size = ModuleFiles::S_Load(filePath, &buffer);

	if (size == 0)
		return false;

	// Change Title
	std::string scenePath = " -- CurrentScene: " + filePath;

	ModuleWindow::S_AddTitleExtraInfo(scenePath);

	ModuleCommand::S_CleanCommandQueue();

	json sceneFile = json::parse(buffer);
	RELEASE(buffer);

	Application::Instance()->renderer3D->renderManager.DestroyInstanceRenderers(); // To prevent duplicated instance renderers.
	Application::Instance()->renderer3D->particleManager.RemoveAllEmitters(); // Remove emitters to avoid calling them before deleting them.
	ModuleLayers::DestroyMeshes(); // When all meshes are destroyed, the Instance Renderers get destroyed as well. In this case, we want this to happen BEFORE we Deserialize the scene
									   // If we let it happen afterwards, the old meshes will destroy the new Instance Renderers.
	LayerGame::RemoveAllScripts();
	Lighting::ClearLights();
// Create New GameObject for root GameObject
	if (ModuleLayers::rootGameObject)
		ModuleLayers::rootGameObject->Destroy();

	std::vector<std::pair<GameObject*, uint>> temp;

	// First  create game objects
	for (int i = 0; i < sceneFile.size(); i++)
	{
		uint prefabUID = sceneFile[i]["PrefabUID"];
		//if (loadedPrefabs.count(prefabUID) > 0 && !sceneFile[i]["FirstOnPrefab"]) continue;

		GameObject* g = new GameObject(nullptr, sceneFile[i]["Name"], sceneFile[i]["Tag"], sceneFile[i]["UID"]);
		
		if(sceneFile[i].contains("IsStatic"))
			g->SetIsStatic(sceneFile[i]["IsStatic"]);
		
		g->SetPrefabUID(prefabUID);
		/*if (prefabUID != 0)
		{
			loadedPrefabs[prefabUID].push_back(std::make_pair(g, sceneFile[i]["ParentUID"]));
			tempPrefab.push_back(std::make_pair(g, prefabUID));
		}*/
		temp.push_back(std::make_pair(g, sceneFile[i]["ParentUID"]));
	}

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i].second != 0)
			/*if (temp[i].first->_prefabUID == 0)*/ temp[i].first->SetParent(ModuleLayers::gameObjects[temp[i].second]);
	}

	/*for (int i = 0; i < tempPrefab.size(); i++)
	{
		if (tempPrefab[i].second != 0)
		{
			ResourcePrefab* aux = (ResourcePrefab*)resources[tempPrefab[i].second];
			S_DeserializeFromPrefab(aux->path, tempPrefab[i].first, true);
		}
	}*/

	// then add their components
	// TODO: Script components whould be the last to get serialied, because they may need to reference another components instances.
	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];
			if (componentType == Component::Type::SCRIPT ||
				componentType == Component::Type::MATERIAL ||
				componentType == Component::Type::UI_INPUT ||
				componentType == Component::Type::AGENT)
				continue;
			/*if (temp[i].first->_prefabUID == 0)*/ temp[i].first->AddComponentSerialized(componentType, object[j]);
		}
	}

	// Create UI & Agent
	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];
			if (componentType == Component::Type::MATERIAL || 
				componentType == Component::Type::UI_INPUT ||
				componentType == Component::Type::AGENT)
				temp[i].first->AddComponentSerialized(componentType, object[j]);
			/*if (temp[i].first->_prefabUID == 0)*/ 
		}
	}

	// Create Scripting
	for (int i = 0; i < sceneFile.size(); i++)
	{
		// Create components
		json object = sceneFile[i]["Components"];
		for (int j = 0; j < object.size(); j++)
		{
			Component::Type componentType = object[j]["Type"];

			if (componentType == Component::Type::SCRIPT)
				temp[i].first->AddComponentSerialized(componentType, object[j]);
			/*if (temp[i].first->_prefabUID == 0)*/ 
		}
	}

	/*for (auto& prefab : loadedPrefabs)
	{
		ResourcePrefab* aux = (ResourcePrefab*)resources[prefab.first];
		S_DeserializePrefabsScripts(aux->path, prefab.second);
	}*/

	for (int i = 0; i < sceneFile.size(); i++)
	{
		temp[i].first->SetActive(sceneFile[i]["Active"]);
	}

	ModuleLayers::rootGameObject = temp[0].first;

	Application::Instance()->xml->GetConfigXML().FindChildBreadth("currentScene").node.attribute("value").set_value(filePath.c_str());

	if (LayerGame::S_IsPlaying())
		LayerGame::StartAllScripts();

	return true;
}

void ModuleResourceManager::S_DeleteMetaFile(const std::string& file, bool onlyResources, bool isReimporting)
{
	MetaFile meta = ModuleFiles::S_LoadMeta(file);

	switch (meta.type)
	{
	case ResourceType::MODEL:
	{
		std::vector<std::string> meshFilePaths;

		ModelNode model;
		model.ReadFromJSON(meta.resourcePath);
		GetResourcePath(model, meshFilePaths);

		for (int i = 0; i < meshFilePaths.size(); i++)
		{
			ModuleFiles::S_Delete(meshFilePaths[i]);
		}
		ModuleFiles::S_Delete(meta.resourcePath);
	}
	break;
	case ResourceType::HSCRIPT:
	{
		if (!isReimporting)
		{
			// Destroy the CppScript named like this .h
			// 1: Get script name.
			ResourceScript* hScriptResource = (ResourceScript*)resources[meta.UID];
			if (hScriptResource == nullptr)
				break;

			std::string scriptName = hScriptResource->className;

			// 2: Search for this scirpt inside the current folder
			std::string cppFilePath = ModuleFiles::S_GetFilePath(file);
			cppFilePath += scriptName + ".cpp"; // We destroy the source file, not the meta file, so the meta is destroyed next frame. This avoids an infinite loop.

			// 3: Destroy Cpp file
			ModuleFiles::S_Delete(cppFilePath);

			// 4: Unlink the destroyed script on the ScriptingSLN.
			ModuleFiles::S_RemoveScriptFromDLLSolution(scriptName, false);
		}

	}
	break;
	case ResourceType::CPPSCRIPT:
	{
		if (!isReimporting)
		{
			// Destroy the HScript named like this .cpp
			// 1: Get script name.
			ResourceScript* cppScriptResource = (ResourceScript*)resources[meta.UID];
			if (cppScriptResource == nullptr)
				break;

			std::string scriptName = cppScriptResource->className;

			// 2: Search for this scirpt inside the current folder
			std::string hFilePath = ModuleFiles::S_GetFilePath(file);
			hFilePath += scriptName + ".h"; // We destroy the source file, not the meta file, so the meta is destroyed next frame. This avoids an infinite loop.

			// 3: Destroy h file
			ModuleFiles::S_Delete(hFilePath);

			// 4: Unlink the destroyed script on the ScriptingSLN.
			ModuleFiles::S_RemoveScriptFromDLLSolution(scriptName, true);
		}
	}
	break;
	case ResourceType::ANIMATION:
	case ResourceType::PREFAB: // When reimporting prefabs we dont want to destroy the old file, because it is probably already the new one. 
								//Kinda confusing I know. Ask code lead for clarification.
		break;

	break;
	default:
	{
		ModuleFiles::S_Delete(meta.resourcePath);
	}
	break;
	}
	if (!onlyResources)
		ModuleFiles::S_Delete(file);

	if (!isReimporting)
	{
		if (resources.count(meta.UID) == 1)
		{
			if (resources[meta.UID] != nullptr)
			{
				resources[meta.UID]->Destroy();
				RELEASE(resources[meta.UID]);
			}
			resources.erase(meta.UID);
		}
	}
}

void ModuleResourceManager::S_CreateResource(const MetaFile& metaFile)
{
	if (resources.count(metaFile.UID) != 0)
		return;

	switch (metaFile.type)
	{
	case ResourceType::MESH:
	{
		resources[metaFile.UID] = new ResourceMesh();
	}
	break;
	case ResourceType::ANIMATION:
	{
		resources[metaFile.UID] = new ResourceAnimation();
	}
	break;
	case ResourceType::MODEL:
	{
		ResourceModel* resourceModel = new ResourceModel();
		resources[metaFile.UID] = resourceModel;
		resources[metaFile.UID]->UID = metaFile.UID;
		resourceModel->resourcePath = metaFile.resourcePath;
		resourceModel->modelInfo.ReadFromJSON(resourceModel->resourcePath);
		resourceModel->CreateResourceMeshes();
	}
	break;
	case ResourceType::TEXTURE:
	{
		resources[metaFile.UID] = new ResourceTexture();
	}
	break;
	case ResourceType::HSCRIPT:
	case ResourceType::CPPSCRIPT:
	{
		resources[metaFile.UID] = new ResourceScript();
		// Get class name from path file name
		ResourceScript* res = (ResourceScript*)resources[metaFile.UID];
		res->className = ModuleFiles::S_GetFileName(metaFile.name, false);
		res->debugName = res->className;
	}
	break;
	case ResourceType::PREFAB:
	{
		resources[metaFile.UID] = new ResourcePrefab();
		ResourcePrefab* r = (ResourcePrefab*)resources[metaFile.UID];
		r->path = metaFile.resourcePath;
	}
	break;
	case ResourceType::SHADER:
	{
		ResourceShader* shaderRes = new ResourceShader();
		resources[metaFile.UID] = shaderRes;
		shaderRes->assetsPath = metaFile.assetsPath;
	}
	break;
	case ResourceType::MATERIAL:
	{
		ResourceMaterial* materialRes = new ResourceMaterial();
		resources[metaFile.UID] = materialRes;
		materialRes->assetsPath = metaFile.assetsPath;
	}
	break;
	case ResourceType::VIDEO:
		resources[metaFile.UID] = new ResourceVideo();
		break;
	default:
		Console::S_Log("Cannot create a resource of an undefined meta file!");
		return;
	}

	resources[metaFile.UID]->referenceCount = 0;
	resources[metaFile.UID]->resourcePath = metaFile.resourcePath;
	resources[metaFile.UID]->type = metaFile.type;
	resources[metaFile.UID]->debugName = metaFile.name;
	resources[metaFile.UID]->UID = metaFile.UID;
}

ResourceMesh* ModuleResourceManager::S_CreateResourceMesh(const std::string& filePath, uint UID, const std::string& name, bool load, ResourceModel* model)
{
	if (resources.count(UID) != 0)
		return nullptr;

	ResourceMesh* newResource = new ResourceMesh();
	if (load)
	{
		newResource->meshInfo.LoadFromBinaryFile(filePath);
		newResource->CreateBuffers();
		newResource->CalculateNormalsAndAABB();
	}
	resources[UID] = newResource;
	resources[UID]->debugName = name + ".hmesh";
	resources[UID]->UID = UID;
	resources[UID]->resourcePath = filePath;
	if (model != nullptr)
	{
		newResource->modelUID = model->UID;
		newResource->indexInsideModel = model->modelMeshes.size();
	}

	return newResource;
}

void ModuleResourceManager::S_CreateResourceAnimation(const std::string& filePath, uint UID, const std::string& name, bool load)
{
	if (resources.count(UID) != 0)
		return;

	ResourceAnimation* newResource = new ResourceAnimation();

	if (load)
	{
		newResource->animation.LoadFromBinaryFile(filePath);
	}

	newResource->debugName = name + ".hanim";
	newResource->UID = UID;
	newResource->resourcePath = filePath;

	resources[UID] = newResource;
}

void ModuleResourceManager::S_CreateResourceText(const std::string& filePath, uint UID, const std::string& name, bool load)
{
	if (resources.count(UID) != 0)
		return;

	ResourceTexture* newResource = new ResourceTexture();

	resources[UID] = newResource;
	resources[UID]->debugName = name + ".dds";
	resources[UID]->UID = UID;
	resources[UID]->resourcePath = filePath;

	if (load)
		S_LoadFileIntoResource(newResource);
}

ResourceShader* ModuleResourceManager::S_CreateResourceShader(const std::string& filePath, uint UID, const std::string& name, bool load)
{
	if (resources.count(UID) != 0)
		return (ResourceShader*)resources[UID];

	ResourceShader* newResource = new ResourceShader();
	if (load)
	{
		newResource->shader = Shader(filePath);
	}

	resources[UID] = newResource;
	resources[UID]->debugName = name + ".shader";
	resources[UID]->UID = UID;
	resources[UID]->resourcePath = filePath;

	return newResource;
}

Resource* ModuleResourceManager::S_LoadResource(const uint& UID)
{
	if (resources.count(UID) == 0)
		return nullptr;

	Resource* ret = resources[UID];

	if (ret == nullptr)
		return nullptr;

	// Load resource
	if (ret->referenceCount == 0)
		S_LoadFileIntoResource(ret);

	ret->referenceCount++;

	return ret; // We assume we always find a resource.
}

bool ModuleResourceManager::S_IsResourceCreated(const uint& UID)
{
	return resources.count(UID) == 1;
}

std::vector<Resource*> ModuleResourceManager::S_GetResourcePool(ResourceType type)
{
	std::vector<Resource*> toReturn;
	for (const auto& r : resources)
	{
		if (r.second == nullptr)
			continue;

		if (r.second->type == type) 
			toReturn.push_back(r.second);
	}

	return toReturn;
}

void ModuleResourceManager::GetResourcePath(ModelNode& node, std::vector<std::string>& vector)
{
	if (node.meshPath != "N")
		vector.push_back(node.meshPath);

	for (int i = 0; i < node.children.size(); i++)
	{
		GetResourcePath(node.children[i], vector);
	}
}

void ModuleResourceManager::SerializeSceneRecursive(const GameObject* g, json& j)
{
	json _j;

	_j["ParentUID"] = g->_parent ? g->_parent->_ID : 0;
	_j["UID"] = g->_ID;
	_j["Name"] = g->name;
	_j["Tag"] = g->tag;
	_j["Active"] = g->_isActive;
	_j["PrefabUID"] = g->_prefabUID;
	_j["IsStatic"] = g->_isStatic;
	if (g->_prefabUID != 0)
		_j["FirstOnPrefab"] = false;
	else
		_j["FirstOnPrefab"] = true;

	// We delay the serialization of script components because they may need to reference another component when Deserialized.
	// this way, ScriptComponents will always deserialize last, and will find any other component they need inside their game object.
	std::vector<ScriptComponent*> scriptComponents;
	for (int i = 0; i < g->_components.size(); i++)
	{
		if (g->_components[i]->GetType() == Component::Type::SCRIPT)
		{
			scriptComponents.push_back((ScriptComponent*)g->_components[i]);
			continue;
		}
		// Serialize components
		g->_components[i]->Serialization(_j);
	}

	for (int i = 0; i < scriptComponents.size(); ++i)
	{
		scriptComponents[i]->Serialization(_j);
	}

	j.push_back(_j);

	for (int i = 0; i < g->_children.size(); i++)
	{
		// Recursive to serialize children
		if (!g->_children[i]->_isPendingToDelete)
			SerializeSceneRecursive(g->_children[i], j);
	}
}

uint ModuleResourceManager::SerializeToPrefab(const GameObject* g, json& j, uint prefabUID, uint parentUID)
{
	json _j;
	uint _newPrefabUID;
	if (prefabUID == 0) _newPrefabUID = HelloUUID::GenerateUUID();
	else _newPrefabUID = prefabUID;

	_j["ParentUID"] = parentUID;
	_j["UID"] = HelloUUID::GenerateUUID();
	_j["Name"] = g->name;
	_j["Tag"] = g->tag;
	_j["Active"] = g->_isActive;
	_j["PrefabUID"] = _newPrefabUID;

	// We delay the serialization of script components because they may need to reference another component when Deserialized.
	// this way, ScriptComponents will always deserialize last, and will find any other component they need inside their game object.
	std::vector<ScriptComponent*> scriptComponents;
	for (int i = 0; i < g->_components.size(); i++)
	{
		if (g->_components[i]->GetType() == Component::Type::SCRIPT)
		{
			scriptComponents.push_back((ScriptComponent*)g->_components[i]);
			continue;
		}
		// Serialize components
		g->_components[i]->Serialization(_j);
	}

	for (int i = 0; i < scriptComponents.size(); ++i)
	{
		scriptComponents[i]->Serialization(_j);
	}

	j.push_back(_j);

	for (int i = 0; i < g->_children.size(); i++)
	{
		// Recursive to serialize children
		if (!g->_children[i]->_isPendingToDelete)
			SerializeToPrefab(g->_children[i], j, _newPrefabUID, _j["UID"]);
	}

	return _newPrefabUID;
}

void ResourceModel::CreateResourceMeshes()
{
	if (modelInfo.meshPath != "N")
	{
		std::string stringUID = ModuleFiles::S_GetFileName(modelInfo.meshPath, false);
		uint meshUID = std::stoul(stringUID);
		if (!ModuleResourceManager::S_IsResourceCreated(meshUID))
		{
			ModuleResourceManager::S_CreateResourceMesh(modelInfo.meshPath, meshUID, modelInfo.name, false, this);
			modelMeshes.push_back((ResourceMesh*)ModuleResourceManager::resources[meshUID]);
		}
	}
	for (int i = 0; i < modelInfo.children.size(); i++)
	{
		CreateResourceMeshesRecursive(modelInfo.children[i]);
	}
}

void ResourceModel::UpdateResourceMeshes()
{
	// Iterate ModelInfo to Reload all Mesh information, but load it into the corresponding already created Resource, if it exists.
	if (modelInfo.meshPath != "N")
	{
		std::string stringUID = ModuleFiles::S_GetFileName(modelInfo.meshPath, false);
		uint meshUID = std::stoul(stringUID);

		// If this mesh is inside the old mesh array dimensions, update old mesh with new mesh information
		if (currentLoadedMesh < modelMeshes.size())
		{
			ResourceMesh* resource = modelMeshes[currentLoadedMesh];

			resource->UnLoad();
			ModuleResourceManager::resources.erase(resource->UID);
			//resource->referenceCount = 0;
			//if (resource->referenceCount > 0)
			//{
			//	resource->meshInfo.LoadFromBinaryFile(modelInfo.meshPath);
			//	resource->CreateBuffers();
			//	resource->CalculateNormalsAndAABB();
			//}
			resource->debugName = modelInfo.name + ".hmesh";
			resource->UID = meshUID;
			resource->resourcePath = modelInfo.meshPath;
			resource->modelUID = UID;
			ModuleResourceManager::resources[resource->UID] = resource;
		}
		else // If not, create a new Mesh Resource
		{
			ModuleResourceManager::S_CreateResourceMesh(modelInfo.meshPath, meshUID, modelInfo.name, false, this);
			modelMeshes.push_back((ResourceMesh*)ModuleResourceManager::resources[meshUID]);
		}
		currentLoadedMesh++;
	}

	for (int i = 0; i < modelInfo.children.size(); i++)
	{
		UpdateResourceMeshesRecursive(modelInfo.children[i]);
	}
}

void ResourceModel::Destroy()
{
	for (int i = 0; i < modelMeshes.size(); ++i)
	{
		if (modelMeshes[i] != nullptr)
		{
			modelMeshes[i]->Destroy();
			ModuleResourceManager::resources.erase(modelMeshes[i]->UID);
			RELEASE(modelMeshes[i]);
		}
	}
	modelMeshes.clear();
}

void ResourceModel::ReImport(const std::string& filePath)
{
	resourcePath = filePath;
	modelInfo.Reset();
	modelInfo.ReadFromJSON(filePath);
	currentLoadedMesh = 0;
	UpdateResourceMeshes();
	for (int i = currentLoadedMesh; i < modelMeshes.size(); ++i)
	{
		modelMeshes[i]->Destroy();
		ModuleResourceManager::resources.erase(modelMeshes[i]->UID);
		RELEASE(modelMeshes[i]);
	}
}

void ResourceModel::CreateResourceMeshesRecursive(ModelNode& node)
{
	// Iterate model. Create resource mesh per mesh inside model.
	if (node.meshPath != "N")
	{
		std::string stringUID = ModuleFiles::S_GetFileName(node.meshPath, false);
		uint meshUID = std::stoul(stringUID);
		if (!ModuleResourceManager::S_IsResourceCreated(meshUID))
		{
			ModuleResourceManager::S_CreateResourceMesh(node.meshPath, meshUID, node.name, false, this);
			modelMeshes.push_back((ResourceMesh*)ModuleResourceManager::resources[meshUID]);
		}
	}

	for (int i = 0; i < node.children.size(); i++)
	{
		CreateResourceMeshesRecursive(node.children[i]);
	}
}

void ResourceModel::UpdateResourceMeshesRecursive(ModelNode& node)
{
	// Iterate model. Create resource mesh per mesh inside model.
	if (node.meshPath != "N")
	{
		std::string stringUID = ModuleFiles::S_GetFileName(node.meshPath, false);
		uint meshUID = std::stoul(stringUID);

		// If this mesh is inside the old mesh array dimensions, update old mesh with new mesh information
		if (currentLoadedMesh < modelMeshes.size())
		{
			ResourceMesh* resource = modelMeshes[currentLoadedMesh];
			//resource->referenceCount = 0;
			ModuleResourceManager::resources.erase(resource->UID);
			resource->UnLoad();

			/*if (resource->referenceCount > 0)
			{
				resource->meshInfo.LoadFromBinaryFile(node.meshPath);
				resource->CreateBuffers();
				resource->CalculateNormalsAndAABB();
			}*/
			resource->debugName = node.name + ".hmesh";
			resource->UID = meshUID;
			resource->resourcePath = node.meshPath;
			resource->modelUID = UID;
			ModuleResourceManager::resources[resource->UID] = resource;
		}
		else // If not, create a new Mesh Resource
		{
			ModuleResourceManager::S_CreateResourceMesh(node.meshPath, meshUID, node.name, false, this);
			modelMeshes.push_back((ResourceMesh*)ModuleResourceManager::resources[meshUID]);
		}
		currentLoadedMesh++;
	}

	for (int i = 0; i < node.children.size(); i++)
	{
		UpdateResourceMeshesRecursive(node.children[i]);
	}
}

void ResourceMesh::CreateBuffers()
{
	// Create Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshInfo.vertices.size(), &meshInfo.vertices.front(), GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	if (meshInfo.boneDataMap.size() != 0)
	{
		// bone Ids
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIds));
		// bone weights
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
	}

	// Create Index Buffer Object
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshInfo.indices.size(), &meshInfo.indices.front(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void ResourceMesh::CalculateNormalsAndAABB()
{
	// Vertex normals
	vertexNormals.resize(meshInfo.vertices.size() * 2);
	float lineMangitude = 0.5f;

	int j = 0;
	for (int i = 0; i < meshInfo.vertices.size() * 2; i++)
	{
		if (i % 2 == 0)
		{
			vertexNormals[i] = meshInfo.vertices[j].position;
		}
		else
		{
			vertexNormals[i] = meshInfo.vertices[j].position + (meshInfo.vertices[j].normals * lineMangitude);
			j++;
		}
	}

	// Face normals
	faceNormals.resize((meshInfo.indices.size() / 3) * 2); // 3 vertices make a face; we need 2 points to display 1 face normal. 
	lineMangitude = 0.5f;
	int k = 0;
	int l = 0;

	int iterations = faceNormals.size() / 2;
	for (int i = 0; i < iterations; i++)
	{
		float3 faceCenter = { 0,0,0 };
		for (int j = 0; j < 3; j++)
		{
			faceCenter += meshInfo.vertices[meshInfo.indices[k++]].position;
		}
		faceCenter /= 3;
		faceNormals.push_back(faceCenter);

		float3 normalsDir = { 0,0,0 };
		for (int j = 0; j < 3; j++)
		{
			normalsDir += meshInfo.vertices[meshInfo.indices[l++]].normals;
		}
		normalsDir /= 3;
		normalsDir.Normalize();
		faceNormals.push_back(faceCenter + (normalsDir * lineMangitude));
	}

	// AABB
	localAABB.SetNegativeInfinity();

	std::vector<float3> vertexPositions;
	vertexPositions.resize(meshInfo.vertices.size());
	for (int i = 0; i < meshInfo.vertices.size(); i++)
	{
		vertexPositions[i] = meshInfo.vertices[i].position;
	}
	localAABB.Enclose(&vertexPositions[0], meshInfo.vertices.size());

	// OpenGL buffers
	// vertex normals buffers
	glGenVertexArrays(1, &VertexNormalsVAO);
	glBindVertexArray(VertexNormalsVAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &VertexNormalsVBO);

	glBindBuffer(GL_ARRAY_BUFFER, VertexNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * vertexNormals.size(), &vertexNormals[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

	// Face normals buffers
	glGenVertexArrays(1, &FaceNormalsVAO);
	glBindVertexArray(FaceNormalsVAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &FaceNormalsVBO);

	glBindBuffer(GL_ARRAY_BUFFER, FaceNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * faceNormals.size(), &faceNormals[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

MeshInfo ResourceMesh::GetMeshInfo()
{
	return meshInfo;
}

void ResourceMesh::Destroy()
{
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		MeshRenderComponent* meshComponent = gameObject.second->GetComponent<MeshRenderComponent>();
		if (meshComponent)
		{
			if (meshComponent->GetResourceUID() == this->UID)
				meshComponent->DestroyedResource();
		}
	}

	RenderManager* renderManager = &Application::Instance()->renderer3D->renderManager;

	for (auto& instanceRenderer : renderManager->_renderMap)
	{
		if (instanceRenderer.second.resource == nullptr)
			continue;

		if (instanceRenderer.second.resource->UID == this->UID)
		{
			instanceRenderer.second.resource = nullptr;
			instanceRenderer.second.deletedResourceUID = this->UID;
		}
	}
}

void ResourceMesh::ReImport(const std::string& filePath)
{
}

void ResourceAnimation::Destroy()
{

}

void ResourceAnimation::ReImport(const std::string& filePath)
{

}

void ResourceTexture::Destroy()
{
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		TextureComponent* materialComponent = gameObject.second->GetComponent<TextureComponent>();
		if (materialComponent)
		{
			if (materialComponent->GetResourceUID() == this->UID)
				materialComponent->DestroyedResource();
		}
	}
}

void ResourceTexture::ReImport(const std::string& filePath)
{
	UnLoad();

	resourcePath = filePath;

	// Only when reference count is > 0 ?
	if (referenceCount > 0)
	{
		char* ddsBuffer = nullptr;
		uint ddsSize = ModuleFiles::S_Load(filePath, &ddsBuffer);
		// Load image
		TextureImporter::Load(ddsBuffer, ddsSize, this);

		RELEASE_ARRAY(ddsBuffer);
	}
}

void ResourceScript::Destroy()
{
	for (auto& gameObject : ModuleLayers::gameObjects)
	{
		std::vector<Component*> components = gameObject.second->GetComponents();

		for (int i = 0; i < components.size(); ++i)
		{
			if (components[i]->GetType() == Component::Type::SCRIPT)
			{
				ScriptComponent* script = (ScriptComponent*)components[i];

				if (script->GetResourceUID() == this->UID)
					script->DestroyedResource();
			}
		}
	}
}

void ResourceMaterial::ReImport(const std::string& filePath)
{
	material.LoadJSON(assetsPath);
	Save();
}

void ResourceMaterial::Save()
{
	json j;
	material.Save(j);

	std::string buffer = j.dump(4);
	ModuleFiles::S_Save(resourcePath, buffer.data(), buffer.length(), false);
	ModuleFiles::S_Save(assetsPath, buffer.data(), buffer.length(), false);
}

void ResourceShader::ReImport(const std::string& filePath)
{
	char* buffer = nullptr;
	uint size = ModuleFiles::S_Load(assetsPath, &buffer);

	if (buffer)
	{
		ModuleFiles::S_Save(resourcePath, buffer, size, false);

		RELEASE_ARRAY(buffer);
	}
}
