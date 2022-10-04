#include "Headers.h"
#include "MeshRenderComponent.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModelRenderManager.h"
#include "RenderManager.h"
#include "Mesh.h"

MeshRenderComponent::MeshRenderComponent()
{
}

MeshRenderComponent::~MeshRenderComponent()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);
	manager->GetMap().erase(_instanceID);
}

void MeshRenderComponent::InitAsDefaultCube(float3 position, float3 scale)
{
	_meshID = 0;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	if (!manager->initialized)
	{
		Mesh cube;
		cube.InitAsCube(position, scale);
		_instanceID = manager->SetMeshInformation(cube);
	}
	else
	{

		Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		_instanceID = manager->AddMesh(meshInfo);
	}
}

void MeshRenderComponent::InitAsDefaultSphere(float3 position, float3 scale)
{
	_meshID = 1;
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);


	if (!manager->initialized)
	{
		Mesh sphere;
		sphere.InitAsSphere(position, scale);
		_instanceID = manager->SetMeshInformation(sphere);
	}
	else
	{

		Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		_instanceID = manager->AddMesh(meshInfo);
	}
}

void MeshRenderComponent::InitAsLoadedMesh(uint meshID)
{
	_meshID = meshID;

	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh instanceMesh;
	instanceMesh.InitAsMeshInformation({ 0.0f,0.0,0.0f }, {1.0f,1.0f,1.0f});

	_instanceID = manager->AddMesh(instanceMesh);

	//TODO: determine how this funciton will be called. 
	// The body of this funciton should:
	// Acces the ModelRenderManager to check if the given mesh has already been loaded.
	// Create an instance inside the RenderManager in ModelRenderManager that manages this unique Mesh.
	// Get an ID to access that instance, so its attributes can be dynamically changed from this component.
}

void MeshRenderComponent::InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Mesh newMesh;
	newMesh.InitAsMesh(vertices, indices, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });

	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID); // Create a renderManager.

	_meshID = manager->SetMeshInformation(newMesh);

}

Mesh& MeshRenderComponent::GetMesh()
{
	RenderManager* manager = Application::Instance()->renderer3D->modelRender.GetRenderManager(_meshID);

	Mesh& meshReference = manager->GetMap()[_instanceID];

	return meshReference;
}
