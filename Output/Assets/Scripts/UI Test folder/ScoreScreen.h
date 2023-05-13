#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ScoreScreen : HelloBehavior
{
	enum class ScreenType
	{
		TO_LEVEL2,
		WIN,
		LOSE
	};
public:
	void Start() override; 
	void Update() override;

	API_UIButton continueButton;


	int screenType = 1;
};

