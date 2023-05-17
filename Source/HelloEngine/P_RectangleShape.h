#pragma once
#include "P_ShapeModule.h"
class P_RectangleShape : public P_ShapeModule
{
public:

	P_RectangleShape();
	~P_RectangleShape();

	void OnEditor() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	float3 GetRandomPos() override;
	bool IsInside(float3 position) override;

	float c1, c2;
	float3 center;
};

