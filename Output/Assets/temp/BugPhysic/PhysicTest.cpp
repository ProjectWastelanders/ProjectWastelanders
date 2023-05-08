#include "PhysicTest.h"
HELLO_ENGINE_API_C PhysicTest* CreatePhysicTest(ScriptToInspectorInterface* script)
{
	PhysicTest* classInstance = new PhysicTest();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void PhysicTest::Start()
{
	rig = gameObject.GetRigidBody();

	API_Vector3 vec = API_Vector3(1, 0, 0);

	Console::Log(vec);

	rig.SetVelocity(vec);

	Console::Log("hey BUG!!!");
}
void PhysicTest::Update()
{
	API_Vector3 vec = API_Vector3(1, 0, 0);

	rig.SetVelocity(vec);
}