#pragma once
#include "P_ShapeModule.h"
class P_RectangleShape : public P_ShapeModule
{
public:

	P_RectangleShape();
	~P_RectangleShape();

	void OnEditor() override;

	float3 GetRandomPos() override;
	bool IsInside(float3 position) override;

	float c1, c2;
	float3 center;
};

