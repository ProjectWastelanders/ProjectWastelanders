#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "ModuleResourceManager.h"


struct ParticleAnimInfo
{
	float4 textOffsets;
	float2 texInfo;
};

struct RenderEntry;
/// <summary>
/// This class manages the rendering process o a single MeshObject.
/// A MeshObject is a colection of Meshes that follow the same vertices/indices pattern.
/// TLDR: This class instance renders a MeshObject.
/// </summary>
class InstanceRenderer
{
public:
	InstanceRenderer();
	~InstanceRenderer();

	void SetMeshInformation(ResourceMesh* resMes, ResourceMaterial* resMat);

	void Draw();
	void DrawMaterial();
	void DrawRaw();

	void Draw2D();

	uint AddMesh();

	std::map<uint, RenderEntry>& GetMap() { return meshes; };

	int GetMeshVertexNum() { return totalVertices->size(); }
	int GetMeshIndexNum() { return totalIndices->size(); }

	/// Draws an instance with an individual draw call.
	void DrawInstance(Mesh* mesh, bool useBasicShader = true);

	void DrawInstancedSorting();

	void DrawInstancedSortingAnimated();

	void SetAs2D();
	uint GetRenderID();

public:
	bool initialized = false;
	bool isParticle = false;
	bool isParticleAnimated = false;
	bool sortedAndDrawn = false;
	ResourceMesh* resource = nullptr;
	ResourceMaterial* resMat = nullptr;
	uint deletedResourceUID = 0;
private:
	void CreateBuffers();
	void CreateBasicBuffers(); // Creates buffers for individual drawing.

	void ReallocateMoreMemory();
	void DestroyDynamicBuffers();
	void CreateDynamicBuffers();

	void CreateDynamicBuffersParticles();
	void DestroyDynamicBuffersParticles();

private:

	ResourceShader* instancedDepthShader = nullptr;

	ResourceShader* instancedShader = nullptr;
	ResourceShader* perMeshShader = nullptr;
	ResourceShader* mesh2DShader = nullptr;
	ResourceShader* particleShader = nullptr;

	std::map<uint, std::pair<uint, RenderEntry>> matMesh;
	std::map<uint, RenderEntry> meshes;
	std::vector<Vertex>* totalVertices = nullptr;
	std::vector<uint>* totalIndices = nullptr;
	std::vector<float4x4> modelMatrices;
	std::vector<float> textureIDs;
	std::vector<ParticleAnimInfo> particleAnimInfos;
	std::multimap<float, Mesh*> orderedMeshes;

	uint VAO = 0; // Vertex Array
	uint VBO = 0; // Vertex buffer
	uint IBO = 0; // Elements buffer object
	uint MBO = 0; // ModelMatrix buffer object
	uint TBO = 0; // TextureID buffer object 
	uint PBO = 0; // Particle Buffer object

	// Buffers to be able to draw a single instance with an individual draw call.
	uint BasicVAO = 0;
	uint BasicVBO = 0;
	uint BasicIBO = 0;

	bool drawVertexNormals = false;
	bool drawFaceNormals = true;

	int instanceNum = 4; // Number of instances available in this RenderManager.

	int IDcounter = 0;

	// 2D drawing

	bool is2D = false;

	bool depthDraw = true;

	Application* _app = nullptr;

	friend class RenderManager;
	friend class MeshRenderComponent;
	friend class Emitter;
};

