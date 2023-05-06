#include "Coleccionable_buttons.h"
HELLO_ENGINE_API_C Coleccionable_buttons* CreateColeccionable_buttons(ScriptToInspectorInterface* script)
{
	Coleccionable_buttons* classInstance = new Coleccionable_buttons();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Coleccionable_buttons::Start()
{

}
void Coleccionable_buttons::Update()
{

}