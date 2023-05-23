#include "ExclamationMarkPull.h"
HELLO_ENGINE_API_C ExclamationMarkPull* CreateExclamationMarkPull(ScriptToInspectorInterface* script)
{
	ExclamationMarkPull* classInstance = new ExclamationMarkPull();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ExclamationMarkPull::Start()
{
	_marksCount = gameObject.GetChildren(_marks);
}
void ExclamationMarkPull::Update()
{

}

void ExclamationMarkPull::ActivateMark(API_GameObject parent)
{
	//_marks[_currentMark]
}
