#pragma once
#include "LightComponent.h"

struct PointLight : public Light
{
	PointLight() : Light(Component::Type::POINT_LIGHT) {}

	float linear = 1.0f;
	float exp = 0.0f;

	float3 position;
	float distance = 16.0f;
};

class PointLightComponent : public LightComponent
{
public:
	PointLightComponent(GameObject* gameObject);
	~PointLightComponent();

	void SerializationUnique(json& j) override;
	void DeSerializationUnique(json& j) override;

	void OnTransformCallback(float4x4 worldMatrix) override;

	void UpdateToLightMap() override;

#ifdef STANDALONE
	void OnEditorUnique() override;

	void MarkAsAlive() override;
#endif

	PointLight data;

	PointLight& GetData() override { return data; }
	void SetData(PointLight& data) { this->data = data; }
};
	

