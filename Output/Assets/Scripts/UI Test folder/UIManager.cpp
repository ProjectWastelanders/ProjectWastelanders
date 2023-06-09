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

	script->AddDragBoxUIInput("Settings Panel", &classInstance->settingsPanel);

	script->AddDragBoxUIButton("Go Back Button", &classInstance->goBack);

	script->AddCheckBox("Is in HUB", &classInstance->inHUB);

	return classInstance;
}

void UIManager::Start()
{
	currentPanel = CurrentPanel::NONE;
	isPaused = true;
	HUB_UIManager::ClosePanel(); // Do this in case the static state of the panel is blocked.
	map = (Map*)mapPanel.GetParent().GetScript("Map");
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
		else if (currentPanel == CurrentPanel::PAUSE)
		{
			ContinueGame();
		}
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN)
	{
		if (currentPanel != CurrentPanel::PAUSE && currentPanel != CurrentPanel::SETTINGS && !inHUB)
		{
			bool hasMap = currentPanel != CurrentPanel::MAP;

			if (hasMap)
			{
				Audio::Event("minimap_open");

				mapPanel.SetActive(hasMap);
				if (map != nullptr)
					map->MissionsEnable(hasMap);
			}
			else
			{
				Audio::Event("minimap_close");

				if (map != nullptr)
					map->MissionsEnable(hasMap);

				mapPanel.SetActive(hasMap);
			}
			HUDPanel.SetActive(!hasMap);
			currentPanel = hasMap ? CurrentPanel::MAP : CurrentPanel::NONE;
		}
		if (currentPanel == CurrentPanel::SETTINGS)
		{
			//CloseSettings();
		}
	}
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_UP)
	{
		//Console::Log("ITS B");
		settingsPanel.SetEnable(false);
		if (currentPanel == CurrentPanel::SETTINGS)
		{
			//Console::Log("ITS B && SETTINGS");
			CloseSettings();
		}
		else if (currentPanel == CurrentPanel::PAUSE)
		{
			ContinueGame();
		}
	}

	if (initialTextConinue.OnPress())
	{
		initialText.SetActive(false);
	}

	if (goBack.OnPress())
	{
		settingsPanel.SetEnable(false);
		CloseSettings();
	}

	if (currentPanel == CurrentPanel::MAP)
	{
		//@Roger queda en revisó
		//Audio::Event("minimap_ambience");
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
	settingsPanel.SetEnable(true);
	//settingsPanel.SetActive(true);
	pausePanel.SetActive(false);
	settingsPanel.GetGameObject().GetTransform().SetPosition(0, 0, 0);

	currentPanel = CurrentPanel::SETTINGS;
	Console::Log("Show Settings");
}

void UIManager::CloseSettings()
{
	settingsPanel.SetEnable(false);
	pausePanel.SetActive(true);
	settingsPanel.GetGameObject().GetTransform().SetPosition(25, 0, 0);

	currentPanel = CurrentPanel::PAUSE;
	Console::Log("Close Settings");
}
