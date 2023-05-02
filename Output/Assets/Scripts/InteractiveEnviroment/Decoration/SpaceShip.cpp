#include "SpaceShip.h"
HELLO_ENGINE_API_C SpaceShip* CreateSpaceShip(ScriptToInspectorInterface* script)
{
	SpaceShip* classInstance = new SpaceShip();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxMeshRenderer("Mesh1", &classInstance->mesh1);
	script->AddDragBoxMeshRenderer("Mesh2", &classInstance->mesh2);
	script->AddDragBoxTransform("Initial Point", &classInstance->point1);
	script->AddDragBoxTransform("Final Point", &classInstance->point2);
	script->AddDragFloat("Speed", &classInstance->speed);
	return classInstance;
}

void SpaceShip::Start()
{
	cd =  rand() % 18 + 7;
	mesh1.SetActive(false);
	mesh2.SetActive(false);

	API_Vector3 normalizedvector = point2.GetGlobalPosition() - point1.GetGlobalPosition();
	float x = normalizedvector.x * normalizedvector.x;
	float y = 0;
	float z = normalizedvector.z * normalizedvector.z;
	float sum = x + y + z;
	direction = { normalizedvector.x / sum, 0, normalizedvector.z / sum };
}

void SpaceShip::Update()
{
	if (!moving)
	{
		cd -= Time::GetDeltaTime();
		if (cd <= 0.0f)
		{
			cd = 0.0f;
			moving = true;
			gameObject.GetTransform().SetPosition(point1.GetGlobalPosition());
			mesh1.SetActive(true);
			mesh2.SetActive(true);
		}
	}
	else
	{
		gameObject.GetTransform().Translate(direction * speed * Time::GetDeltaTime());
		if (gameObject.GetTransform().GetGlobalPosition().Distance(point2.GetGlobalPosition()) < 1.0f)
		{
			cd = rand() % 120 + 30;
			moving = false;
			mesh1.SetActive(false);
			mesh2.SetActive(false);
		}
	}
}