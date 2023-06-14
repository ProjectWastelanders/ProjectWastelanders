#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include"API/API_UIImage.h"

class UIManager;
class PlayerMove;
class PlayerStorage;

class PauseMenu : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void OnDisable() override;

	API_GameObject uiManager;
	API_GameObject player;

	API_UIButton continueButton;
	API_UIButton settingsButton;
	API_UIButton mainMenuButton;
	UIManager* manager;
	PlayerMove* blockMovement;
	PlayerStorage* saveLevel;
	string currentLevel;
};

