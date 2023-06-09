#include "HUB_LevelSelect.h"
#include "HUB_UIManager.h"
HELLO_ENGINE_API_C HUB_LevelSelect* CreateHUB_LevelSelect(ScriptToInspectorInterface* script)
{
	HUB_LevelSelect* classInstance = new HUB_LevelSelect();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Level 1 button: ", &classInstance->levels[0]);
	script->AddDragBoxUIButton("Level 2 button: ", &classInstance->levels[1]);
	script->AddDragBoxUIButton("Level 3 button: ", &classInstance->levels[2]);
	script->AddDragBoxUIButton("Level 4 button: ", &classInstance->levels[3]);

	script->AddDragBoxGameObject("SelectedLevel1", &classInstance->selectedLevelImage[0]);
	script->AddDragBoxGameObject("SelectedLevel2", &classInstance->selectedLevelImage[1]);
	script->AddDragBoxGameObject("SelectedLevel3", &classInstance->selectedLevelImage[2]);
	script->AddDragBoxGameObject("SelectedLevel4", &classInstance->selectedLevelImage[3]);

	script->AddDragBoxGameObject("Level1 Info", &classInstance->levelInfo[0]);
	script->AddDragBoxGameObject("Level2 Info", &classInstance->levelInfo[1]);
	script->AddDragBoxGameObject("Level3 Info", &classInstance->levelInfo[2]);
	script->AddDragBoxGameObject("Level4 Info", &classInstance->levelInfo[3]);

	script->AddDragBoxGameObject("Hala1", &classInstance->floorTexts[0]);
	script->AddDragBoxGameObject("Hala2", &classInstance->floorTexts[1]);
	script->AddDragBoxGameObject("Santuary", &classInstance->floorTexts[2]);
	script->AddDragBoxGameObject("Thanos", &classInstance->floorTexts[3]);

	script->AddDragBoxGameObject("Tutorial 1", &classInstance->tutorials[0]);
	script->AddDragBoxGameObject("Tutorial 2", &classInstance->tutorials[1]);
	script->AddDragBoxGameObject("Tutorial 3", &classInstance->tutorials[2]);
	script->AddDragBoxUIInput("Panel", &classInstance->panel);

	script->AddCheckBox("Unlocked levels", &classInstance->lockedLevels);

	return classInstance;
}

void HUB_LevelSelect::Init()
{
	for (int i = 0; i < 4; ++i)
	{
		std::string name = "level" + std::to_string(i + 1) + "_completed";
		if (API_QuickSave::GetBool(name, false)) // Find which is the highest completed level
		{
			Console::Log("completed!");
			currentSelectedLevel = i +1;
			if (currentSelectedLevel > 3)
				currentSelectedLevel = 3;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		floorTexts[i].SetActive(currentSelectedLevel == i);
	}
}

void HUB_LevelSelect::Start()
{
	if (!API_QuickSave::GetBool("SelectLevel_Tutorial", false))
	{
		tutorial = new UITutorial(tutorials, 3);
		tutorials[0].SetActive(true);
		tutorials[1].SetActive(false);
		tutorials[2].SetActive(false);
		Console::Log("Tut!");
	}
	else
	{
		tutorial = nullptr;
		tutorials[0].SetActive(false);
		tutorials[1].SetActive(false);
		tutorials[2].SetActive(false);
	}

	for (int i = 0; i < 4; ++i)
	{
		// initialize images
		API_GameObject children[2];
		selectedLevelImage[i].GetChildren(children, 2);
		for (int j = 0; j < 2; ++j)
		{
			// Get API_Image component
			API_UIImage image = children[j].GetUIImage();
			if (image.IsAlive())
				holdBarsSelectedLevel[i] = image;
		}
		// read unlocked levels
		if (!lockedLevels)
		{
			unlockedLevels[0] = true;
			unlockedLevels[1] = API_QuickSave::GetBool("level1_completed", false);
			unlockedLevels[2] = API_QuickSave::GetBool("level2_completed", false);
			unlockedLevels[3] = API_QuickSave::GetBool("level3_completed", false);
		}
		else
		{
			unlockedLevels[0] = true;
			unlockedLevels[1] = true;
			unlockedLevels[2] = true;
			unlockedLevels[3] = true;
		}

	}

	levels[currentSelectedLevel].SetBlocked(true);
}

void HUB_LevelSelect::Update()
{
	if (tutorial != nullptr)
	{
		panel.SetEnable(false);
		if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
		{
			if (!tutorial->NextScreen())
			{
				tutorial->Unable();
				delete tutorial;
				tutorial = nullptr;
				panel.SetEnable(true);
				API_QuickSave::SetBool("SelectLevel_Tutorial", true);
			}
		}
		return;
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			tutorials[i].SetActive(false);
		}
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		gameObject.SetActive(false);
		HUB_UIManager::ClosePanel();
		return;
	}

	pressedButtonThisFrame = false;
	currentHoveredLevel = -1;
	// Iterate all buttons
	for (int i = 0; i < 4; ++i)
	{
		// Activate progress bar of currently selected button
		if (levels[i].OnHovered())
		{
			if (currentSelectedLevel != i)
				selectedLevelImage[i].SetActive(true);

			currentHoveredLevel = i;
		}

		if (levels[i].OnPress())
		{
			currentHoveredLevel = i;
			if (currentSelectedLevel == i) // If we are already selecting this, only mark it as currently being hovered and ignore the other funcitonality.
				continue;
			if (!unlockedLevels[i])
				continue;

			Audio::Event("spaceshipTable_button_hold");
		}

		// If this level is unlocked 
		if (levels[i].OnHold() || levels[i].OnPress())
		{
			currentHoveredLevel = i;
			if (currentSelectedLevel == i) // If we are already selecting this, only mark it as currently being hovered and ignore the other funcitonality.
				continue;
			if (!unlockedLevels[i])
				continue;

			pressedButtonThisFrame = true;

			selectedLevelImage[i].SetActive(true); // make sure the bar is active when holding
			currentTime += Time::GetDeltaTime();
			float currentProgress = currentTime / totalTime; // Progress in a 0 to 1 scale.
			holdBarsSelectedLevel[i].FillImage(currentProgress);
			// Select level
			if (currentTime >= totalTime)
			{
				SelectLevel(i);
				currentTime = 0.0f;
				holdBarsSelectedLevel[i].FillImage(0);
			}
			continue;
		}

		selectedLevelImage[i].SetActive(false); // Deactivate progress bar of other buttons.
	}

	UpdateInfoAndProgressBar();

}

void HUB_LevelSelect::UpdateInfoAndProgressBar()
{
	// If no button was pressed this frame, reset all bars to 0 to be sure, and set current press time to 0.
	if (!pressedButtonThisFrame)
	{
		currentTime = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			holdBarsSelectedLevel[i].FillImage(0);
		}
	}

	if (currentHoveredLevel == -1)
		currentHoveredLevel = currentSelectedLevel;

	// Active current hovered level info.
	for (int i = 0; i < 4; ++i)
	{
		levelInfo[i].SetActive(currentHoveredLevel == i);
	}
}

void HUB_LevelSelect::SelectLevel(uint level)
{
	currentSelectedLevel = level;
	API_QuickSave::SetInt("currentSelectedLevel", level);
	for (int i = 0; i < 4; ++i)
	{
		levels[i].SetBlocked(level == i);
		floorTexts[i].SetActive(level == i);
	}
}
