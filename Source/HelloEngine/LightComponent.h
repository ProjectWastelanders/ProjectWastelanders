#pragma once
#include "Component.h"


struct Light
{
	Light() {};
	Light(Component::Type type)
	{
		_type = type;
	}

	Component::Type GetType() { return this->_type; }

	float3 color = float3(1.0f, 1.0f, 1.0f);
	float ambientIntensity = 1.0f;
	float diffuseIntensity = 1.0f;

	bool isEnabled = true;

protected:
	Component::Type _type = Component::Type::LIGHT;
};

class LightComponent : public Component
{
public:
	LightComponent(GameObject* gameObject);
	~LightComponent();

	void Serialization(json& _j) override;
	virtual void SerializationUnique(json& j);
	void DeSerialization(json& _j) override;
	virtual void DeSerializationUnique(json& _j);

	void OnEnable() override;
	void OnDisable() override;

#ifdef STANDALONE
	void OnEditor() override;

	virtual void OnEditorUnique();

	void MarkAsDead() override;
#endif

public:
	Light data;

	std::string _name;
	uint _lightID;

public:
	virtual Light& GetData() { return data; }
	virtual void SetData(Light& data) { this->data = data; }

protected:
	void UpdateData(Light& data);
	virtual void UpdateToLightMap();
};

