#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ScoreScreen : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton continueButton;
	API_UIButton returnButton;

	int lvlToSpawn = 1;

	bool hasToStart;
};

