#include "PlayCinematic.h"
#include "Audio/MusicManager.h"
HELLO_ENGINE_API_C PlayCinematic* CreatePlayCinematic(ScriptToInspectorInterface* script)
{
	PlayCinematic* classInstance = new PlayCinematic();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxAudioSourceComponent("Audio Source", &classInstance->audio);
	script->AddDragBoxVideoPlayerComponent("VideoPlayer", &classInstance->video);
	script->AddDragBoxUIButton("Close button", &classInstance->closeButton);
	script->AddDragInt("Cinematic num", &classInstance->cinematicNum);
	script->AddInputBox("From level check", &classInstance->quicksaveCheck);
	return classInstance;
}

void PlayCinematic::Start()
{
	std::string string = "Cinematic_Comlpeted" + std::to_string(cinematicNum);
	std::string quicksaveCheckString = quicksaveCheck.c_str();
	if (!API_QuickSave::GetBool(string) && (API_QuickSave::GetBool(quicksaveCheckString, false) || quicksaveCheckString == ""))
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
	if (closeButton.OnPress() || video.VideoEnded())
	{
		audio.Stop();
		video.Stop();
		this->gameObject.SetActive(false);

		API_QuickSave::SetBool("Cinematic_Comlpeted" + std::to_string(cinematicNum), true);

		MusicManager* music = (MusicManager*)Game::FindGameObject("MusicManager").GetScript("MusicManager");
		if (music != nullptr)
		{
			music->mainMusic.Play();
			music->playingCinematic = false;
		}
	}
}