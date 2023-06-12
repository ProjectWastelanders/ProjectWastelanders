#include "IntroPlay.h"
HELLO_ENGINE_API_C IntroPlay* CreateIntroPlay(ScriptToInspectorInterface* script)
{
	IntroPlay* classInstance = new IntroPlay();
	script->AddDragBoxVideoPlayerComponent("Video", &classInstance->video);
	script->AddDragBoxAudioSourceComponent("Audio", &classInstance->audio);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void IntroPlay::Start()
{
	video.Stop();
	video.Play();
	audio.Play();
}
void IntroPlay::Update()
{
	if (video.VideoEnded() || Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
	{
		Scene::LoadScene("MainMenu.HScene");
	}
}