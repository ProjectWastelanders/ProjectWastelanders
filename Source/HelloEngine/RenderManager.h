#pragma once

#include "InstanceRenderer.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "MeshRenderComponent.h"
#include "PhysBody3D.h"
#include "FontManager.h"

#define MAX_VERTICAL_SLICES_SPHERE 32
#define MAX_HORIZONTAL_SLICES_SPHERE 32
#define MAX_VERTICAL_SLICES_CYLINDER 32

enum class PrimitiveType
{
	CUBE,
	SPHERE,
	CYLINDER,
	PLANE,
	PLANE2D, // Used in UI
};

enum class UIType
{
	BUTTON,
	CHECKBOX,
	SLIDER,
	IMAGE,
	TEXT,
	VIDEO,
};

enum class PrimitiveModelsUID
{
	CUBE = 10,
	SPHERE = 11,
	CYLINDER = 12,
	PLANE = 13,
	PLANE2D = 14,
};

struct RenderEntry
{
	uint resMat = 0;
	ResourceMaterial* material = nullptr;
	Mesh mesh;
	float distance = 0.0f;
};

/// <summary>
/// This class contains a colletion of RenderManagers. It dynamically creates and destroys Render Managers tu fullfill the task of having one per Unique mesh.
/// Every Render Manager updates and draws their corresponding Models.
/// </summary>
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	void Init();

	void OnEditor();

	InstanceRenderer* GetRenderManager(uint meshID, uint materialID, bool create = true);
	uint GetMapSize() { return _renderMap.size(); };

	void Draw();
	void DrawDebug();
	void Draw2D();

	uint AddMesh(ResourceMesh* resource, uint resMat, MeshRenderType type);

	uint AddTransparentMesh(ResourceMesh* resource, uint resMat);
	uint AddIndependentMesh(ResourceMesh* resource, uint resMat);
	uint AddInstancedMesh(ResourceMesh* resource, uint resMat);
	uint Add2DMesh();
	uint AddTextObject(std::string text = "Default Text", float4 color = { 1,1,1,1 }, float2 position = { 0, 0 }, float scale = 1.0f);

	void CreatePrimitive(GameObject* parent, PrimitiveType type);
	void CreateUI(GameObject* parent, UIType type);

	void DestroyRenderManager(uint managerUID);

	void SetSelectedMesh(RenderEntry* mesh);
	void SetSelectedMesh(Mesh* mesh);
	void DrawSelectedMesh();
	void RemoveSelectedMesh();

	void DrawVertexNormals(Mesh* mesh);
	void DrawFaceNormals(Mesh* mesh);
	void DrawOBB(Mesh* mesh);
	void DrawAABB(Mesh* mesh);
	void DrawColliderBox(PhysBody3D* phsyBody, float4 color = { 0.5f, 0.0f, 0.5f, 1.0f }, float wireSize = 3.0f);
	void DrawColliderSphere(std::vector<float3>* spherePointsComp, std::vector<uint>* sphereIndicesComp, float4 color = { 0.5f, 0.0f, 0.5f, 1.0f }, float wireSize = 1.0f);
	void DrawColliderCylinder(std::vector<float3>* cylinderPointsComp, std::vector<uint>* cylinderIndicesComp, float4 color, float wireSize);

	void CalculateSphereBuffer(std::vector<uint>* sphereIndicesComp, uint verSlices = 16, uint horSlices = 16);
	void CalculateCylinderBuffer(std::vector<uint>* cylinderIndicesComp, uint verSlices = 16);

	void CalculateSphereIndices(std::vector<uint>* sphereIndicesComp, uint verSlices, uint horSlices);
	void CalculateSpherePoints(PhysBody3D* physBody, std::vector<float3>* spherePointsComp, float radius, uint verSlices, uint horSlices);

	void CalculateCylinderIndices(std::vector<uint>* sphereIndicesComp, uint verSlices);
	void CalculateCylinderPoints(PhysBody3D* physBody, std::vector<float3>* cylinderPointsComp, float2 radiusHeight, uint verSlices);
	
	void DrawRayCastLine(float3 pos1, float3 pos2, float4 color, float lineSize);

	void DestroyInstanceRenderers();

	void DrawTransparentMeshes();
	void DrawIndependentMeshes();
	void DrawTextObjects();

private:
	std::map<uint, InstanceRenderer> _renderMap; // Render managers that use instance rendering to draw opaque meshes.
	std::map<uint, RenderEntry> _transparencyMeshes; // Meshes with transparency that must be drawn with a draw call per mesh.
	std::multimap<float, RenderEntry*> _orderedMeshes; // Meshes with transparency ordered from furthest to closest to the camera.

	std::map<uint, RenderEntry> _independentMeshes; // Opaque meshes that need to be drawn in an independent draw call.

	TextureManager* _textureManager = nullptr;

	std::vector<uint> _emptyRenderManagers;

	RenderEntry* _selectedMesh = nullptr;
	Mesh* _selectedMeshRaw = nullptr;

	std::vector<uint> boxIndices; // Used to display bounding boxes.
	/*std::vector<uint> sphereIndices;*/
	/*std::vector<uint> cylinderIndices;*/
	std::vector<uint> sphereIndicesMax;
	std::vector<uint> cylinderIndicesMax;
	std::vector<uint> rayCastLineIndices;

	// ModelResources for primitives
	ResourceModel* primitiveModels[5];

	// Shaders for drawing debug information
	ResourceShader* lineShader = nullptr;
	ResourceShader* localLineShader = nullptr;
	ResourceShader* instancedShader = nullptr;
	Shader* textRenderingShader = nullptr;

	uint AABBVAO = 0;
	uint AABBVBO = 0;

	uint OBBVAO = 0;
	uint OBBVBO = 0;

	uint SPVAO = 0;
	uint SPVBO = 0;

	uint CYVAO = 0;
	uint CYVBO = 0;

	uint RAYVAO = 0;
	uint RAYVBO = 0;

	uint AABBIBO = 0; // index buffer object shared by both OBB and ABB buffers above.
	uint OBBIBO = 0;
	uint SPIBO = 0;
	uint CYIBO = 0;
	uint RAYIBO = 0;

	// Primitives
	uint cubeUID = 0;
	uint sphereUID = 0;
	uint planeUID = 0;
	uint cylinderUID = 0;
	uint plane2DUID = 0;

	// Text rendering
	std::map<uint, TextObject> textObjects;

	uint TextVAO = 0;
	uint TextVBO = 0;
	uint TextIBO = 0;

	InstanceRenderer* renderer2D = nullptr;

	bool drawDepthIndependent = true;

	friend class Emitter;
	friend class ParticleSystemComponent;
	friend class MeshRenderComponent;
	friend class ResourceMesh;
	friend class Mesh;
	friend class TextRendererComponent;
};

