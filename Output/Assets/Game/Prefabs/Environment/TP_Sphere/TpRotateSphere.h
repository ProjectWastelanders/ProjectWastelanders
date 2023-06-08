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
	float deformationTime = 0.0f;
	float secondsToDeform = 0.0f;
	float deformationScale = 0.0f;

	bool deformedVer = false;
	bool inInFirstPart = false;
	int partsCount = 0;

	float sphereSize = 0.0f;
};

