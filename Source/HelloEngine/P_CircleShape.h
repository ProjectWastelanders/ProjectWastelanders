#pragma once
#include "P_ShapeModule.h"

class P_CircleShape : public P_ShapeModule
{
public:

	P_CircleShape();
	~P_CircleShape();

	void OnEditor() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	float3 GetRandomPos() override;
	bool IsInside(float3 position) override;

	float radius;
	float3 center;
};

