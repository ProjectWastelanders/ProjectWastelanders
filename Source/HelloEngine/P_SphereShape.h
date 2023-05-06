#pragma once
#include "P_ShapeModule.h"
class P_SphereShape : public P_ShapeModule
{
public:

	P_SphereShape();
	~P_SphereShape();

	void OnEditor() override;

	float3 GetRandomPos() override;
	bool IsInside(float3 position) override;

	float radius;
	float3 center;
};

