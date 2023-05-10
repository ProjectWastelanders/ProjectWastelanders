#include "IslandMovement.h"
#include <string>

HELLO_ENGINE_API_C IslandMovement* CreateIslandMovement(ScriptToInspectorInterface* script)
{
	IslandMovement* classInstance = new IslandMovement();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	
	for (size_t i = 0; i < ISLAND_NUM; i++)
	{
		std::string n = "island_" + std::to_string(i);
		std::string p = "final_pos_" + std::to_string(i+1);

		script->AddDragBoxGameObject(n.c_str(), &classInstance->islands[i]);
		script->AddDragBoxTransform(p.c_str(), &classInstance->positions[i]);
	}

	return classInstance;
}

void IslandMovement::Start()
{

}

void IslandMovement::Update()
{
	if (Input::GetKey(KeyCode::KEY_M) == KeyState::KEY_DOWN) MoveIslands();
}

void IslandMovement::MoveIslands()
{
	for (size_t i = 0; i < ISLAND_NUM; i++)
	{
		if (islands[i].IsAlive())
		{
			API_Transform t = islands[i].GetTransform();
			t.SetPosition(positions[i].GetGlobalPosition());
		}
	}
}