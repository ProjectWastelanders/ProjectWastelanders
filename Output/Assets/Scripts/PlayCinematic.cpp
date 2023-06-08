#include "PlayCinematic.h"
#include "Audio/MusicManager.h"
HELLO_ENGINE_API_C PlayCinematic* CreatePlayCinematic(ScriptToInspectorInterface* script)
{
	PlayCinematic* classInstance = new PlayCinematic();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxAudioSourceComponent("Audio Source", &classInstance->audio);
	script->AddDragBoxVideoPlayerComponent("VideoPlayer", &classInstance->video);
	script->AddDragBoxUIButton("Close button", &classInstance->closeButton);
	return classInstance;
}

void PlayCinematic::Start()
{
	if (!API_QuickSave::GetBool("Cinematic_Comlpeted"))
	{
		video.GetGameObject().SetActive(true);
		audio.GetGameObject().SetActive(true);
		closeButton.GetGameObject().SetActive(true);
		video.Play();
		audio.Play();
		MusicManager* music = (MusicManager*)Game::FindGameObject("MusicManager").GetScript("MusicManager");
		if (music != nullptr)
		{
			music->playingCinematic = true;
			music->mainMusic.Stop();
		}
	}

	
}
void PlayCinematic::Update()
{
	if (closeButton.OnPress())
	{
		audio.Stop();
		video.Stop();
		this->gameObject.SetActive(false);

		API_QuickSave::SetBool("Cinematic_Comlpeted", true);

		MusicManager* music = (MusicManager*)Game::FindGameObject("MusicManager").GetScript("MusicManager");
		if (music != nullptr)
		{
			music->mainMusic.Play();
			music->playingCinematic = false;
		}
	}
}