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
	API_RayCast apiRaycast = rigidBody.RayCastLineLocal(40, API_Vector3(0, 4, 0), API_Vector3(4, 0, 0));

	if (apiRaycast.GetCollisionsSize() > 0) 
	{	
		std::vector<API_RayCastCollision> collisionsVec;
		collisionsVec.resize(apiRaycast.GetCollisionsSize());
		apiRaycast.GetCollisionsInRaycast(&collisionsVec[0]);
		
		Console::Log("GameObject Vector Size is: " + to_string(collisionsVec.size()));
		for (int i = 0; i < collisionsVec.size(); i++)
		{

			Console::Log("GameObject At (" + to_string(i) + "): Name is: "+ collisionsVec.at(i).GetGameObject().GetName());
			Console::Log("GameObject At (" + to_string(i) + "): Fraction is: " + to_string(collisionsVec.at(i).GetFraction()));
			Console::Log("GameObject At (" + to_string(i) + "): Distance is: " + to_string(collisionsVec.at(i).GetDistance()));
			Console::Log("GameObject At (" + to_string(i) + "): Point is: (" + to_string(collisionsVec.at(i).GetPoint().x) + " " + to_string(collisionsVec.at(i).GetPoint().y) + " " + to_string(collisionsVec.at(i).GetPoint().z) + ")");
			Console::Log("GameObject At (" + to_string(i) + "): Normal is: (" + to_string(collisionsVec.at(i).GetNormal().x) + " " + to_string(collisionsVec.at(i).GetNormal().y) + " " + to_string(collisionsVec.at(i).GetNormal().z) + ")");
		}
	}
}