#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HUB_LevelSelect;

class HUB_MissionsCheck : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject checksParent;
	API_GameObject checks[3];
	API_String missionsNames[3];
};

