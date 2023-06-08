#include "Hub_Audio_Ambience.h"
HELLO_ENGINE_API_C Hub_Audio_Ambience* CreateHub_Audio_Ambience(ScriptToInspectorInterface* script)
{
	Hub_Audio_Ambience* classInstance = new Hub_Audio_Ambience();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Hub_Audio_Ambience::Start()
{

}
void Hub_Audio_Ambience::Update()
{
	Audio::Event("ambience_hub");
}