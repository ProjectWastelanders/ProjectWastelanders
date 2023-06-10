#include "hub_ambience_music.h"
HELLO_ENGINE_API_C hub_ambience_music* Createhub_ambience_music(ScriptToInspectorInterface* script)
{
	hub_ambience_music* classInstance = new hub_ambience_music();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void hub_ambience_music::Start()
{
	Audio::Event("ambience_hub");
}
void hub_ambience_music::Update()
{
	
}