#pragma once

#include "Mesh.h"
#include "Component.h"
#include "ModuleResourceManager.h"

enum class MeshRenderType
{
	INSTANCED,
	INDEPENDENT,
	TRANSPARENCY,
	MESH2D,
};

class MeshRenderComponent : public Component
{
public:
	MeshRenderComponent(GameObject* gameObject);
	MeshRenderComponent(GameObject* gameObject, const MeshRenderComponent& copy);
	virtual ~MeshRenderComponent() override;

	virtual void CreateMesh(uint resourceUID, uint materialUID = 0, MeshRenderType type = MeshRenderType::INSTANCED);

	void CreateMesh2D();

	void OnTransformCallback(float4x4 worldMatrix) override;

	void OnEnable() override;
	void OnDisable() override;

	Mesh& GetMesh();

	void DestroyMesh();

	std::vector<Vertex>& GetMeshVertices();
	std::vector<uint>& GetMeshIndices();

	/// Changes mesh render type to Transparency, Independent or Instance. 
	void ChangeMeshRenderType(MeshRenderType type);
	MeshRenderType GetMeshRenderType();

	void SetAs2D();

	bool HasBones() { return _hasBones; }

#if STANDALONE
	void OnEditor() override;

	void MarkAsDead() override;
	void MarkAsAlive() override;
#endif // STANDALONE

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	ResourceMesh* GetResource();

	uint GetResourceUID();
	void DestroyedResource();

	void UnlinkResource();

protected:
	void MeshDropArea();

protected:
	ResourceMesh* _resource = nullptr;
	int64_t _meshID = -1;
	uint _instanceID = 0;
	uint _renderID = 0; //ID of the stored map.

	int _selectedNormalDisplay = 0;

	int _vertexNum = 0;
	int _indexNum = 0;

	bool _isTransparent = false;
	bool _hasBones = false;
	MeshRenderType renderType = MeshRenderType::INSTANCED;

	// Temporal. Delete later:
	std::string _comboOptions[3] = { "INSTANCE", "INDEPENDENT", "TRANSPARENCY" };
	uint currentCombo = 0;
	uint _resourceUID = 0;

	// 2D rendering
	bool is2D = false;

	friend class Mesh;
};