#include "Coleccionable_buttons.h"
HELLO_ENGINE_API_C Coleccionable_buttons* CreateColeccionable_buttons(ScriptToInspectorInterface* script)
{
	Coleccionable_buttons* classInstance = new Coleccionable_buttons();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Button 1", &classInstance->button1);
	script->AddDragBoxUIButton("Button 2", &classInstance->button2);
	script->AddDragBoxUIButton("Button 3", &classInstance->button3);
	

	return classInstance;
}

void Coleccionable_buttons::Start()
{

}
void Coleccionable_buttons::Update()
{

}