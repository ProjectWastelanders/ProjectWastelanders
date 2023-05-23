#include "ExclamationMarkPull.h"
HELLO_ENGINE_API_C ExclamationMarkPull* CreateExclamationMarkPull(ScriptToInspectorInterface* script)
{
	ExclamationMarkPull* classInstance = new ExclamationMarkPull();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ExclamationMarkPull::Start()
{
	//gameObject.GetChildren();
}
void ExclamationMarkPull::Update()
{

}