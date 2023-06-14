#include "PlayCreditsMainMenu.h"
#include "../MenuButtons.h"
HELLO_ENGINE_API_C PlayCreditsMainMenu* CreatePlayCreditsMainMenu(ScriptToInspectorInterface* script)
{
	PlayCreditsMainMenu* classInstance = new PlayCreditsMainMenu();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxVideoPlayerComponent("Video", &classInstance->video);
	script->AddDragBoxAudioSourceComponent("Audio", &classInstance->audio);
	script->AddDragBoxUIButton("ExitButton", &classInstance->exitButton);
	script->AddDragBoxUIInput("MainMenuPanel", &classInstance->mainPanel);
	script->AddDragBoxUIInput("CreditsPanel", &classInstance->creditsPanel);
	return classInstance;
}


void PlayCreditsMainMenu::Update()
{
	justPlayed = false;
	if (!playing)
		return;

	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN || video.VideoEnded() || exitButton.OnPress())
	{
		playing = false;
		video.Reset();
		audio.Stop();
		video.GetGameObject().SetActive(false);
		audio.GetGameObject().SetActive(false);
		mainPanel.SetEnable(true);
		creditsPanel.SetEnable(false);
		MenuButtons* menubuttons = (MenuButtons*)mainPanel.GetGameObject().GetParent().GetScript("MenuButtons");
		menubuttons->shouldActivate = true;
		justPlayed = true;
	}
}

void PlayCreditsMainMenu::PlayCinematic()
{
	video.GetGameObject().SetActive(true);
	audio.GetGameObject().SetActive(true);
	video.Reset();
	audio.Stop();
	mainPanel.SetEnable(false);
	creditsPanel.SetEnable(true);

	video.Play();
	audio.Play();
	playing = true;
}
