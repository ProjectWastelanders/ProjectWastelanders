#include "MusicManager.h"
HELLO_ENGINE_API_C MusicManager* CreateMusicManager(ScriptToInspectorInterface* script)
{
	MusicManager* classInstance = new MusicManager();
	script->AddDragBoxAudioSourceComponent("Main Music Component", &classInstance->mainMusic);
	script->AddDragBoxAudioSourceComponent("Casette 1 Component", &classInstance->casetteMusic[0]);
	script->AddDragBoxAudioSourceComponent("Casette 2 Component", &classInstance->casetteMusic[1]);
	script->AddDragBoxAudioSourceComponent("Casette 3 Component", &classInstance->casetteMusic[2]);

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void MusicManager::Start()
{
	mainMusic.Play();
}
void MusicManager::Update()
{
	if (!casettePlaying)
		return;

	if (!casetteMusic[currentPlayingCassette].IsPlaying())
	{
		mainMusic.Play();
		casettePlaying = false;
	}
}

void MusicManager::PlayCasette(int casetteNum)
{
	switch (casetteNum)
	{
	case 0:
		mainMusic.Stop();
		Console::Log("Play Casette 1");
		casetteMusic[casetteNum].Play();
		break;
	case 1:
		mainMusic.Stop();
		Console::Log("Play Casette 2");
		casetteMusic[casetteNum].Play();
		break;	
	case 2:
		mainMusic.Stop();
		Console::Log("Play Casette 3");
		casetteMusic[casetteNum].Play();
		break;
	}
	casettePlaying = true;
	currentPlayingCassette = casetteNum;
}
