#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class GrootAnimationScript : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_AnimationPlayer grootAnimationPlayer;
	uint grootEscape;
};

