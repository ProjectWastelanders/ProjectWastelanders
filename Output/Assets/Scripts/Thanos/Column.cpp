#include "Column.h"
HELLO_ENGINE_API_C Column* CreateColumn(ScriptToInspectorInterface* script)
{
	Column* classInstance = new Column();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Column::Start()
{
	gameObject.GetRigidBody().SetVelocity({ 0,0,0 });
}
void Column::Update()
{

}