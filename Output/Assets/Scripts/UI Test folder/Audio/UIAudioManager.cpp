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

	script->AddDragBoxGameObject("Master Ojbect", &classInstance->masterBackground);
	script->AddDragBoxGameObject("SFX Ojbect", &classInstance->sfxBackground);
	script->AddDragBoxGameObject("Music Ojbect", &classInstance->musicBackground);

	script->AddDragBoxTextureResource("Master Idle", &classInstance->masterIdle);
	script->AddDragBoxTextureResource("Master Hover", &classInstance->masterHover);
	script->AddDragBoxTextureResource("SFX Idle", &classInstance->sfxIdle);
	script->AddDragBoxTextureResource("SFX Hover", &classInstance->sfxHover);
	script->AddDragBoxTextureResource("Music Idle", &classInstance->musicIdle);
	script->AddDragBoxTextureResource("Music Hover", &classInstance->musicHover);

	script->AddDragBoxUICheckBox("VSync CheckBox", &classInstance->VSync);

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

	masterHovered = false;
	sfxHovered = false;
	musicHovered = false;
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
		if (!masterHovered)
		{
			sfxBackground.GetMaterialCompoennt().ChangeAlbedoTexture(sfxIdle);
			masterBackground.GetMaterialCompoennt().ChangeAlbedoTexture(masterHover);
			masterHovered = true;
			sfxHovered = false;
		}
		masterText.SetText(std::to_string(masterSlider.GetValue()).c_str());
	}

	if (sfxSlider.OnHovered())
	{
		if (!sfxHovered)
		{
			masterBackground.GetMaterialCompoennt().ChangeAlbedoTexture(masterIdle);
			sfxBackground.GetMaterialCompoennt().ChangeAlbedoTexture(sfxHover);
			musicBackground.GetMaterialCompoennt().ChangeAlbedoTexture(musicIdle);
			masterHovered = false;
			sfxHovered = true;
			musicHovered = false;
		}
		sfxText.SetText(std::to_string(sfxSlider.GetValue()).c_str());
	}

	if (musicSlider.OnHovered())
	{
		if (!musicHovered)
		{
			sfxBackground.GetMaterialCompoennt().ChangeAlbedoTexture(sfxIdle);
			musicBackground.GetMaterialCompoennt().ChangeAlbedoTexture(musicHover);
			sfxHovered = false;
			musicHovered = true;
		}
		musicText.SetText(std::to_string(musicSlider.GetValue()).c_str());
	}

	if (VSync.OnHovered())
	{
		if (musicHovered)
		{
			musicBackground.GetMaterialCompoennt().ChangeAlbedoTexture(musicIdle);
			musicHovered = false;
		}
	}
}