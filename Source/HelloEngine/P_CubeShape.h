#pragma once
#include "P_ShapeModule.h"
class P_CubeShape : public P_ShapeModule
{
public:
	P_CubeShape();
	~P_CubeShape();

	void OnEditor() override;

	float3 GetRandomPos() override;
	bool IsInside(float3 position) override;

	float c1, c2, c3;
	float3 center;
};

