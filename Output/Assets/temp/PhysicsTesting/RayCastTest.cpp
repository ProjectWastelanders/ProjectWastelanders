#include "RayCastTest.h"
HELLO_ENGINE_API_C RayCastTest* CreateRayCastTest(ScriptToInspectorInterface* script)
{
	RayCastTest* classInstance = new RayCastTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void RayCastTest::Start()
{
	rigidBody = gameObject.GetRigidBody();
}
void RayCastTest::Update()
{
	rigidBody.RayCastLineLocal(10, API_Vector3(0, 4, 0), API_Vector3(4, 0, 0));
}