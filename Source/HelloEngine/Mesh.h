#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Math/float3.h"
#include "Shader.h"
#include "Material.h"


#define MAX_BONE_WEIGHTS 4
#define MAX_BONES 100

class MeshRenderComponent;
class ResourceMesh;
class ResourceShader;

enum class OpacityDirection
{
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	UP_TO_DOWN,
	DOWN_TO_UP
};

enum RenderUpdateState
{
	NODRAW = -1,
	DRAW = 0,
	SELECTED
};

struct Vertex
{
	Vertex() {}
	Vertex(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), texCoords(texC){}
	float3 position = { 0,0,0 };
	float3 normals = { 0,0,0 };
	float2 texCoords = { 0,0 };

	//Bone indexes that influence this vertex
	int boneIds[MAX_BONE_WEIGHTS] = {-1, -1, -1, -1};
	//Weights from each bone 
	float weights[MAX_BONE_WEIGHTS] = {0, 0, 0, 0};
};

struct BoneData
{
	//The final index in the boneMatrix
	int id;

	//Offset matrix. Transform model's vertex to bone space.
	float4x4 offset;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Only to be used for meshes that cannot be drawn with instanced rendering (meshes with transparency).
	void CreateBufferData();
	void Draw(Material material, bool useMaterial = true);
	// ----------------------------------------------------------------------------------------------------
private:
	void DefaultDraw();

	void UniformDraw(Material material);

	void StencilDraw();
public:
	void DrawAsSelected(Material material, uint materialID = 0);
	void DrawAsSelected();

	void InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);
	void InitWithResource(ResourceMesh* res);

	RenderUpdateState Update();

	void CleanUp();

	void CalculateBoundingBoxes();

	void BonesStep(Material& material);

public:

	std::vector<Vertex>* _vertices = nullptr;
	std::vector<uint>* _indices = nullptr;
	float4x4 modelMatrix = modelMatrix.identity;
	
	float OpenGLTextureID = -1;
	float textureID = -1;

	bool showVertexNormals = false;
	bool showFaceNormals = false;
	bool showAABB = false;
	bool showOBB = false;

	bool draw = true;
	bool outOfFrustum = false;

	OBB globalOBB;
	AABB globalAABB;
	AABB localAABB;

	bool isIndependent = false;
	bool is2D = false;

	// TODO: Change this when refacotring the UI rendering pipeline!! TEMPORAL CODE FOR ALPHA 1
	float opacity = 1.0f;
	float opacityLimit = 1.0f;
	OpacityDirection opacityDir = OpacityDirection::LEFT_TO_RIGHT;

	float4 color = { 1.0f,1.0f,1.0f,1.0f };

private:
	ResourceMesh* resource = nullptr;
	MeshRenderComponent* component = nullptr;
	bool _updateMatrix = true;

	// Only to be used for meshes that cannot be drawn with instanced rendering (meshes with transparency).
	uint _VAO = 0;
	uint _VBO = 0;
	uint _IBO = 0;
	ResourceShader* drawPerMeshShader = nullptr;
	ResourceShader* drawPerMesh2D = nullptr;
	// ----------------------------------------------------------------------------------------------------
	ResourceShader* boneMeshShader = nullptr;

	ResourceShader* stencilShader = nullptr;
	ResourceShader* depthShader = nullptr;
	ResourceShader* depthBoneShader = nullptr;

	Application* _app = nullptr;

	friend class RenderManager;
	friend class MeshRenderComponent;
	friend class InstanceRenderer;
};

#endif // !__PRIMITIVE_H__