#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "UI Test folder/UITutorial.h"

class Armory_Tutorial : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIInput armoryScreen;
	API_GameObject tutorials[2];

	UITutorial* tutorial = nullptr;
};

