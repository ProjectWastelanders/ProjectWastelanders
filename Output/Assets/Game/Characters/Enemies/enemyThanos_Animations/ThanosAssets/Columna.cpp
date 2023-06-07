#include "Columna.h"
HELLO_ENGINE_API_C Columna* CreateColumna(ScriptToInspectorInterface* script)
{
	Columna* classInstance = new Columna();
	script->AddDragInt("Column number", &classInstance->numColumn);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Columna::Start()
{

}
void Columna::Update()
{

}