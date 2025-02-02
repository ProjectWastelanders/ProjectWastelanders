#pragma once
#include "P_Module.h"

#define SHAPE_NUM 6

class P_ShapeModule :  public P_Module
{
public:
	P_ShapeModule();
	~P_ShapeModule();

	void OnEditor() override;

	void Serialization(json& j) override;

	void DeSerialization(json& j) override;

	virtual float3 GetRandomPos();

	virtual bool IsInside(float3 position);

	std::string _shapeValues[SHAPE_NUM] = { "Default", "Circle", "Rectangle", "Sphere", "Cube", "Cone"};
};

