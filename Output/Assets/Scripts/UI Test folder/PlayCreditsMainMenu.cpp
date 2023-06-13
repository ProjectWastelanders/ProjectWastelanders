#include "PlayCreditsMainMenu.h"
HELLO_ENGINE_API_C PlayCreditsMainMenu* CreatePlayCreditsMainMenu(ScriptToInspectorInterface* script)
{
	PlayCreditsMainMenu* classInstance = new PlayCreditsMainMenu();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxVideoPlayerComponent("Video", &classInstance->video);
	script->AddDragBoxAudioSourceComponent("Audio", &classInstance->audio);
	return classInstance;
}


void PlayCreditsMainMenu::Update()
{
	if (!playing)
		return;

	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN || video.VideoEnded())
	{
		playing = false;
		video.Stop();
		audio.Stop();
		video.GetGameObject().SetActive(false);
		audio.GetGameObject().SetActive(false);
	}
}

void PlayCreditsMainMenu::PlayCinematic()
{
	video.GetGameObject().SetActive(true);
	audio.GetGameObject().SetActive(true);
	video.Stop();
	audio.Stop();

	video.Play();
	audio.Play();
	playing = true;
}
