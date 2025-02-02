#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../UITutorial.h"

class HUB_LevelSelect : HelloBehavior
{
public:
	void Init() override;
	void Start() override; 
	void Update() override;

	void UpdateInfoAndProgressBar();

	API_UIButton levels[4];
	API_GameObject selectedLevelImage[4];
	API_UIImage holdBarsSelectedLevel[4];

	API_GameObject levelInfo[4];

	API_GameObject floorTexts[4];

	// Progress bar
	float totalTime = 1.5f;
	float currentTime = 0.0f;

	uint currentSelectedLevel = 0;

	bool lockedLevels = false;

	API_GameObject tutorials[3];

	UITutorial* tutorial = nullptr;

	API_UIInput panel;

	bool unlockedLevels[4]; // List of unlocked levels.
	bool pressedButtonThisFrame = false;
	int currentHoveredLevel = -1;

	bool selecting_Level;
	uint audioLoopLevelSelect;

private:
	void SelectLevel(uint level);
};

