#include "PlayerInCombat.h"
#include "PlayerStats.h"
#include "../Enemies/Enemy.h"

HELLO_ENGINE_API_C PlayerInCombat* CreatePlayerInCombat(ScriptToInspectorInterface* script)
{
	PlayerInCombat* classInstance = new PlayerInCombat();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragInt("Num of Enemies", &classInstance->enemiesSize);

	return classInstance;
}

void PlayerInCombat::Start()
{
	enemies.resize(enemiesSize);

	Game::FindGameObjectsWithTag("Enemy", &enemies[0], enemiesSize);

	playerStats = (PlayerStats*)gameObject.GetScript("PlayerStats");

}
void PlayerInCombat::Update()
{

	int numOfEnemiesTargeting = 0;

	for (API_GameObject& var : enemies)
	{
		if (!var.IsActive())continue;

		Enemy* enemyRef = (Enemy*)var.GetScript("Enemy");

		if (enemyRef->targeting)
		{
			numOfEnemiesTargeting++;
		}
	
	}

	if (numOfEnemiesTargeting >= 1)
	{
		playerStats->inCombat = true;
	}
	else
	{
		playerStats->inCombat = false;
	}

}