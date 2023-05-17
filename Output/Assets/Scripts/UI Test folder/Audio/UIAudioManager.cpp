#include "UIAudioManager.h"
HELLO_ENGINE_API_C UIAudioManager* CreateUIAudioManager(ScriptToInspectorInterface* script)
{
	UIAudioManager* classInstance = new UIAudioManager();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIText("Text Master Volume", &classInstance->masterText);
	script->AddDragBoxUIText("Text SFX Volume", &classInstance->sfxText);
	script->AddDragBoxUIText("Text Music Volume", &classInstance->musicText);

	script->AddDragBoxUISlider("Slider Master", &classInstance->masterSlider);
	script->AddDragBoxUISlider("Slider SFX", &classInstance->sfxSlider);
	script->AddDragBoxUISlider("Slider Music", &classInstance->musicSlider);

	return classInstance;
	 
}

void UIAudioManager::Start()
{
	API_GameObject uiObjects[128];
	API::Game::FindGameObjectsWithTag("UI", uiObjects, 128);

	for (int i = 0; i < 128; ++i)
	{
		if (!uiObjects[i].IsAlive())
			break;
		if (uiObjects[i].GetUIButton().IsAlive())
		{
			sceneButtons.push_back(uiObjects[i].GetUIButton());
		}
	}
}
void UIAudioManager::Update()
{
	for (int i = 0; i < sceneButtons.size(); ++i)
	{
		if (!sceneButtons[i].GetGameObject().IsActive())
			continue;
		if (sceneButtons[i].OnPress())
		{
			Audio::Event("click");
		}
		if (sceneButtons[i].OnHoveredFirst())
		{
			Audio::Event("hover");
		}
	}

	if (masterSlider.OnHovered())
	{
		masterText.SetText(std::to_string(masterSlider.GetValue()).c_str());
	}
}