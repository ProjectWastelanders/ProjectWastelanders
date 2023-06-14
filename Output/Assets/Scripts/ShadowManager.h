#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ShadowManager : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void SetShadowsHigh();
	void SetShadowsLow();

	API_DirectionalLight directionalLight;
};

