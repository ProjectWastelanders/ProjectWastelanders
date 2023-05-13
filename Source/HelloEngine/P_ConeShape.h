#pragma once
#include "P_ShapeModule.h"
class P_ConeShape : public P_ShapeModule
{
public:
	P_ConeShape();
	~P_ConeShape();

	void OnEditor() override;

	float3 GetRandomPos() override;
	bool IsInside(float3 position) override;

	float radius, height;
	float3 center;
	bool invert;
	int dir; //1 = X ; 2 = Y ; 3 = Z
};

