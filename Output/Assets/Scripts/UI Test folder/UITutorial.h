#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class UITutorial : HelloBehavior
{
public:
	UITutorial(API_GameObject* tutorials, uint count);

	bool NextScreen();

	void Unable();

	API_GameObject tutorialScreens[4];

	uint currentScreen = 0;
	uint totalScreens = 0;
};

