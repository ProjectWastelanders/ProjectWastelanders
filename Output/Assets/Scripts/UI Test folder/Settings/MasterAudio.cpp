#include "MasterAudio.h"
HELLO_ENGINE_API_C MasterAudio* CreateMasterAudio(ScriptToInspectorInterface* script)
{
	MasterAudio* classInstance = new MasterAudio();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void MasterAudio::Start()
{

}
void MasterAudio::Update()
{

}