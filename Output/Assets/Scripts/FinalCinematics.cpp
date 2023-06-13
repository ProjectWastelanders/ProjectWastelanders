#include "FinalCinematics.h"
HELLO_ENGINE_API_C FinalCinematics* CreateFinalCinematics(ScriptToInspectorInterface* script)
{
	FinalCinematics* classInstance = new FinalCinematics();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxVideoPlayerComponent("Credits", &classInstance->credits);
	script->AddDragBoxVideoPlayerComponent("cinematic4", &classInstance->cinematic4);
	script->AddDragBoxVideoPlayerComponent("cinematic5", &classInstance->cinematic5);

	script->AddDragBoxAudioSourceComponent("AudioCredits", &classInstance->audioCredits);
	script->AddDragBoxAudioSourceComponent("audio4", &classInstance->audio4);
	script->AddDragBoxAudioSourceComponent("audio5", &classInstance->audio5);
	return classInstance;
}

void FinalCinematics::Start()
{
	cinematic4.Reset();
	cinematic4.Play();
	audio4.Stop();
	audio4.Play();
	currentVideo = cinematic4;
}
void FinalCinematics::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN || currentVideo.VideoEnded())
	{
		if (currentVideoNum == 0)
		{
			cinematic4.GetGameObject().SetActive(false);
			audio4.Stop();

			credits.GetGameObject().SetActive(true);
			credits.Reset();
			credits.Play();
			audioCredits.Play();
			currentVideoNum++;
			currentVideo = credits;
		}
		else if (currentVideoNum == 1)
		{
			credits.GetGameObject().SetActive(false);
			audioCredits.Stop();

			cinematic5.GetGameObject().SetActive(true);
			cinematic5.Reset();
			cinematic5.Play();
			audio5.Play();
			currentVideoNum++;
			currentVideo = cinematic5;
		}
		else
		{
			Scene::LoadScene("MainMenu.HScene");
		}
	}
}