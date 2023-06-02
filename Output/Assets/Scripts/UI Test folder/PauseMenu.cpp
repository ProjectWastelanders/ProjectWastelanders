#include "PauseMenu.h"
#include "UIManager.h"
#include "HUB/HUB_UIManager.h"
#include "../Player/PlayerMove.h"

HELLO_ENGINE_API_C PauseMenu* CreatePauseMenu(ScriptToInspectorInterface* script)
{
	PauseMenu* classInstance = new PauseMenu();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("UI Manager: ", &classInstance->uiManager);
	script->AddDragBoxGameObject("Player", &classInstance->player);
	script->AddDragBoxUIButton("Continue: ", &classInstance->continueButton);
	script->AddDragBoxUIButton("Settings: ", &classInstance->settingsButton);
	script->AddDragBoxUIButton("Main Menu: ", &classInstance->mainMenuButton);

	return classInstance;
}

void PauseMenu::Start()
{
	manager = (UIManager*)uiManager.GetScript("UIManager");
	blockMovement = (PlayerMove*)player.GetScript("PlayerMove");
	saveLevel = (PlayerStorage*)player.GetScript("PlayerStorage");
}
void PauseMenu::Update()
{
	blockMovement->openingChest = true;
	if (continueButton.OnPress())
	{
		manager->ContinueGame();
	}
	else if (settingsButton.OnPress())
	{
		manager->ShowSettings();
		blockMovement->openingChest = true;
	}
	else if (mainMenuButton.OnPress())
	{
		switch (saveLevel->levelIndex)
		{
		case 1:
			currentLevel = "Level1.HScene";
			break;

		case 2:
			currentLevel = "Level2.HScene";
			break;

		case 3:
			currentLevel = "Level3.HScene";
			break;

		case 4:
			currentLevel = "Level4.HScene";
			break;
		}
		API_QuickSave::SetString("WhatLevel", currentLevel);
		HUB_UIManager::ClosePanel();
		Time::ChangeTimeScale(1.0f);
		Scene::LoadScene("MainMenu.HScene");
	}
}
void PauseMenu::OnDisable() 
{
	if (blockMovement)
		blockMovement->openingChest = false;
}