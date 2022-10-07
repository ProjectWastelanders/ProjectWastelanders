#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "glmath.h"
#include "Color.h"
#include "Math/float3.h"

struct Vertex
{
	Vertex() {}
	Vertex(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), texCoords(texC){}
	float3 position;
	float3 normals;
	float2 texCoords;
};

//TODO: Every Mesh should have an ID that identifies it's original form, so they can be distributed on diferent render managers.
//TODO: Define what a MeshObject is.
//TODO: Check process of creating the first Mesh inside the Render Manager. Not every Mesh needs the vertices and indices vectors (only the first one does).
class Mesh
{
public:
	Mesh();
	~Mesh();

	void InitAsMeshInformation(float3 position, float3 scale);
	void InitAsCube(float3 position, float3 scale);
	void InitAsSphere(float3 position, float3 scale);

	void InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices, float3 pos, float3 scale);

	void Update();

	void CleanUp();

	float3 scale;
	float3 position;

	std::vector<Vertex>* _vertices = nullptr;
	std::vector<uint>* _indices = nullptr;
	float4x4 modelMatrix;
private:

};

#endif // !__PRIMITIVE_H__