#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class TpRotateSphere : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_Transform trans;
	float timeCount = 0.0f;
	float timeMult = 0.0f;
};

