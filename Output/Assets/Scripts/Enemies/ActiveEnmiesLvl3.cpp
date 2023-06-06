#include "ActiveEnmiesLvl3.h"
HELLO_ENGINE_API_C ActiveEnmiesLvl3* CreateActiveEnmiesLvl3(ScriptToInspectorInterface* script)
{
	ActiveEnmiesLvl3* classInstance = new ActiveEnmiesLvl3();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Enemy to Active 0", &classInstance->enebledEnemies[0]);
	script->AddDragBoxGameObject("Enemy to Active 1", &classInstance->enebledEnemies[1]);
	script->AddDragBoxGameObject("Enemy to Active 2", &classInstance->enebledEnemies[2]);
	script->AddDragBoxGameObject("Enemy to Active 3", &classInstance->enebledEnemies[3]);
	script->AddDragBoxGameObject("Enemy to Active 4", &classInstance->enebledEnemies[4]);
	script->AddDragBoxGameObject("Enemy to Active 5", &classInstance->enebledEnemies[5]);
	script->AddDragBoxGameObject("Enemy to Active 6", &classInstance->enebledEnemies[6]);
	script->AddDragBoxGameObject("Enemy to Active 7", &classInstance->enebledEnemies[7]);
	script->AddDragBoxGameObject("Enemy to Active 8", &classInstance->enebledEnemies[8]);
	script->AddDragBoxGameObject("Enemy to Active 9", &classInstance->enebledEnemies[9]);
	script->AddDragBoxGameObject("Enemy to Active 10", &classInstance->enebledEnemies[10]);
	script->AddDragBoxGameObject("Enemy to Active 11", &classInstance->enebledEnemies[11]);
	script->AddDragBoxGameObject("Enemy to Active 12", &classInstance->enebledEnemies[12]);
	script->AddDragBoxGameObject("Enemy to Active 13", &classInstance->enebledEnemies[13]);
	script->AddDragBoxGameObject("Enemy to Active 14", &classInstance->enebledEnemies[14]);
	script->AddDragBoxGameObject("Enemy to Active 15", &classInstance->enebledEnemies[15]);
	script->AddDragBoxGameObject("Enemy to Active 16", &classInstance->enebledEnemies[16]);
	script->AddDragBoxGameObject("Enemy to Active 17", &classInstance->enebledEnemies[17]);
	script->AddDragBoxGameObject("Enemy to Active 18", &classInstance->enebledEnemies[18]);
	return classInstance;
}

void ActiveEnmiesLvl3::Start()
{
	Game::FindGameObjectsWithTag("Milano", &spaceShipGO,1);
	spaceShip = (ReachTheSpaceship*)spaceShipGO.GetScript("ReachTheSpaceship");
}
void ActiveEnmiesLvl3::Update()
{
	if (!spaceShip)
	{
		if (spaceShip->enabled)
		{
			for (size_t i = 0; i < 17; i++)
			{
				enebledEnemies[i].SetActive(true);
			}
		}
	}
}