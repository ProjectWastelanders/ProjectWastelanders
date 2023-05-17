#include "HUB_LevelSelect.h"
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

	return classInstance;
}

void HUB_LevelSelect::Start()
{
	currentSelectedLevel = API_QuickSave::GetInt("currentSelectedLevel"); // Default value = 0 = LVL1

	for (int i = 0; i < 4; ++i)
	{
		API_GameObject children[2];
		selectedLevelImage[i].GetChildren(children, 2);
		for (int j = 0; j < 2; ++j)
		{
			// Get API_Image component
			API_UIImage image = children[j].GetUIImage();
			if (image.IsAlive())
				holdBarsSelectedLevel[i] = image;
		}
	}

	levels[currentSelectedLevel].SetBlocked(true);
}

void HUB_LevelSelect::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		std::string message = this->gameObject.GetName();
		Console::Log("Curretn game object: " + message);
		this->gameObject.SetActive(false);
		return;
	}

	bool pressedButtonThisFrame = false;
	int currentHoveredLevel = -1;
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

		if (levels[i].OnHold() || levels[i].OnPress()) // Ignore this instructions if this is already the selected level
		{
			currentHoveredLevel = i;
			if (currentSelectedLevel == i) // If we are already selecting this, only mark it as currently being hovered and ignore the other funcitonality.
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
	}
}
