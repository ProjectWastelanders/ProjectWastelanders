#pragma once

class ParticleSystemComponent;

using json = nlohmann::json;

enum class P_ModuleType
{
	MAIN,
	EMISSION,
	SHAPE,
	NONE
};

enum class ShapeType {
	NONE,
	CIRCLE,
	RECTANGLE,
	SPHERE,
	CUBE,
	CONE
};

class P_Module
{
public:

	P_Module();
	virtual ~P_Module();

	virtual void OnEditor();

	virtual void Serialization(json& j);

	virtual void DeSerialization(json& j);

	ParticleSystemComponent* component = nullptr;

	P_ModuleType type;

	ShapeType shapeType;

	friend class ParticleSystemComponent;
};

