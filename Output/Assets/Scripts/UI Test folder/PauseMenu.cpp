#include "PauseMenu.h"
#include "UIManager.h"
#include "HUB/HUB_UIManager.h"
HELLO_ENGINE_API_C PauseMenu* CreatePauseMenu(ScriptToInspectorInterface* script)
{
	PauseMenu* classInstance = new PauseMenu();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("UI Manager: ", &classInstance->uiManager);
	script->AddDragBoxUIButton("Continue: ", &classInstance->continueButton);
	script->AddDragBoxUIButton("Settings: ", &classInstance->settingsButton);
	script->AddDragBoxUIButton("Main Menu: ", &classInstance->mainMenuButton);

	return classInstance;
}

void PauseMenu::Start()
{
	manager = (UIManager*)uiManager.GetScript("UIManager");
}
void PauseMenu::Update()
{
	if (continueButton.OnPress())
	{
		manager->ContinueGame();
	}
	else if (settingsButton.OnPress())
	{
		manager->ShowSettings();
	}
	else if (mainMenuButton.OnPress())
	{
		HUB_UIManager::ClosePanel();
		Time::ChangeTimeScale(1.0f);
		Scene::LoadScene("MainMenu.HScene");
	}
}