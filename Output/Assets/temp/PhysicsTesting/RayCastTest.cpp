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
	API_RayCast apiRaycast = rigidBody.RayCastLineGlobal(40, API_Vector3(0, 4, 0), API_Vector3(4, 0, 0));

	if (apiRaycast.GetObjectsSize() > 0) 
	{
		std::vector<API_GameObject> gameObjectVec;
		gameObjectVec.resize(apiRaycast.GetObjectsSize());
		apiRaycast.GetObjectsInRaycast(&gameObjectVec[0]);
		
		Console::Log("GameObject Vector Size is: " + to_string(gameObjectVec.size()));
		for (int i = 0; i < gameObjectVec.size(); i++) 
		{
			Console::Log("GameObject At (" + to_string(i) + "): Name is: "+ gameObjectVec.at(i).GetName()); 
		}
	}
}