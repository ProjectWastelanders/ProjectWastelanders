#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "ThanosAttacks.h"
#include "ThanosLoop.h"

#include "API/API.h"

class ThanosRecolor : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void FadeIn();
	void FadeOut();
	float Lerp(float a, float b, float time);

	API_ShaderComponent recolor;

	float r = 2.0f;
	float g = 2.0f;
	float b = 2.0f;

	float fadeInTime = 3.0f;
	float fadeOutTime = 3.0f;

	ThanosAttacks* thanosAttacks = nullptr;
	ThanosLoop* thanosLoop = nullptr;

private:
	float _fadeInCooldown;
	float _fadeOutCooldown;
	bool fading;

	float _r = 0;
	float _g = 0;
	float _b = 0;
};

