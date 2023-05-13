#pragma once
#include "Component.h"

class ResourceMaterial;
class MeshRenderComponent;

class MaterialComponent : public Component
{
public:
	MaterialComponent(GameObject* gameObject);
	~MaterialComponent();

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void SetColor(float4 color);
#ifdef STANDALONE
	void OnEditor() override;

	void MarkAsDead() override;
	void MarkAsAlive() override;

#endif // STANDALONE

	int GetResourceUID();

	ResourceMaterial* _resource = nullptr;

private:
	void MaterialDragNDrop();
	void ShaderSelectCombo();

	MeshRenderComponent* GetOwnerMeshComponent();
	
private:

	uint _resourceUID = 0;
};

