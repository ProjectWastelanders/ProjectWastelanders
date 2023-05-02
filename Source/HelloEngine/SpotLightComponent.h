#pragma once
#include "LightComponent.h"

struct SpotLight : public Light
{
	SpotLight() : Light(Component::Type::SPOT_LIGHT) {}

	float linear = 1.0f;
	float exp = 0.0f;

	float3 position;
	float3 direction;

	float cutoff;
	float distance = 16.0f;
};

class SpotLightComponent : public LightComponent
{
public:
	SpotLightComponent(GameObject* gameObject);
	~SpotLightComponent();

	void SerializationUnique(json& j) override;
	void DeSerializationUnique(json& j) override;

	void OnTransformCallback(float4x4 worldMatrix) override;

	void UpdateToLightMap() override;

#ifdef STANDALONE
	void OnEditorUnique() override;

	void MarkAsAlive() override;
#endif

	SpotLight data;

	SpotLight& GetData() override { return data; }
	void SetData(SpotLight& data) { this->data = data; }
};

