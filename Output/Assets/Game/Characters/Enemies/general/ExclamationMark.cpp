#include "ExclamationMark.h"
HELLO_ENGINE_API_C ExclamationMark* CreateExclamationMark(ScriptToInspectorInterface* script)
{
	ExclamationMark* classInstance = new ExclamationMark();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ExclamationMark::Start()
{

}
void ExclamationMark::Update()
{

}

void ExclamationMark::OnEnable()
{
	//gameObject.GetTransform().SetPosition(API_Vector3(1, 1, 1));
}

void ExclamationMark::OnDisable()
{
	//gameObject.GetTransform().SetPosition(API_Vector3(0, 0, 0));
}
