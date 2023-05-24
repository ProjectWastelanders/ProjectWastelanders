#include "UIManager.h"
#include "HUB/HUB_UIManager.h"
HELLO_ENGINE_API_C UIManager* CreateUIManager(ScriptToInspectorInterface* script)
{
	UIManager* classInstance = new UIManager();
	script->AddDragBoxGameObject("Pause Panel", &classInstance->pausePanel);
	script->AddDragBoxGameObject("Map Panel", &classInstance->mapPanel);
	script->AddDragBoxGameObject("HUD Panel", &classInstance->HUDPanel);
	script->AddDragBoxGameObject("Initial text Panel", &classInstance->initialText);
	script->AddDragBoxGameObject("Final text Panel", &classInstance->finalText);
	script->AddDragBoxUIButton("Continute button Initial text", &classInstance->initialTextConinue);
	script->AddDragBoxUIButton("Continute button Final text", &classInstance->finalTextConinue);

	script->AddDragBoxGameObject("Settings Panel", &classInstance->settingsPanel);

	script->AddDragBoxUIButton("Go Back Button", &classInstance->goBack);

	return classInstance;
}

void UIManager::Start()
{
	currentPanel = CurrentPanel::NONE;
	isPaused = true;
	HUB_UIManager::ClosePanel(); // Do this in case the static state of the panel is blocked.
}

void UIManager::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_START) == KeyState::KEY_DOWN)
	{
		if (currentPanel == CurrentPanel::NONE && isPaused && !HUB_UIManager::IsPanelOpened() /*Esto siempre deveulve false si no estas en HUB*/)
		{
			// Call Pause on game
			Time::ChangeTimeScale(0.0f);
			// Show pause menu
			pausePanel.SetActive(true);
			HUDPanel.SetActive(false);
			currentPanel = CurrentPanel::PAUSE;

			HUB_UIManager::OpenPanel();
		}
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN)
	{
		if (currentPanel != CurrentPanel::PAUSE && currentPanel != CurrentPanel::SETTINGS)
		{
			bool hasMap = currentPanel != CurrentPanel::MAP;
			mapPanel.SetActive(hasMap);
			HUDPanel.SetActive(!hasMap);
			currentPanel = hasMap ? CurrentPanel::MAP : CurrentPanel::NONE;
		}
		if (currentPanel == CurrentPanel::SETTINGS)
		{
			//CloseSettings();
		}
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_REPEAT)
	{
		//Console::Log("ITS B");
		settingsPanel.SetActive(false);
		if (currentPanel == CurrentPanel::SETTINGS)
		{
			//Console::Log("ITS B && SETTINGS");
			CloseSettings();
		}
	}

	if (initialTextConinue.OnPress())
	{
		initialText.SetActive(false);
	}

	if (goBack.OnPress())
	{
		settingsPanel.SetActive(false);
		CloseSettings();
	}

}

void UIManager::ContinueGame()
{
	// Call Pause on game
	Time::ChangeTimeScale(1.0f);

	// Show pause menu
	pausePanel.SetActive(false);
	HUDPanel.SetActive(true);

	currentPanel = CurrentPanel::NONE;

	isPaused = true;
	HUB_UIManager::ClosePanel();
}

void UIManager::ShowInitialText()
{
	initialText.SetActive(true);
}

void UIManager::ShowFinalText()
{
	finalText.SetActive(true);
}

void UIManager::ShowSettings()
{
	settingsPanel.SetActive(true);
	pausePanel.SetActive(false);

	currentPanel = CurrentPanel::SETTINGS;
}

void UIManager::CloseSettings()
{
	settingsPanel.SetActive(false);
	pausePanel.SetActive(true);

	currentPanel = CurrentPanel::PAUSE;
}
