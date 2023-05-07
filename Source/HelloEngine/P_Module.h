#pragma once

class ParticleSystemComponent;

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

	ParticleSystemComponent* component = nullptr;

	P_ModuleType type;

	ShapeType shapeType;

	friend class ParticleSystemComponent;
};

