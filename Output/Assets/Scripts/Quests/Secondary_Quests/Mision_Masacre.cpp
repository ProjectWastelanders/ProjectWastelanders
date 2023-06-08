#include "Mision_Masacre.h"
#include "../../Player/PlayerStorage.h"

HELLO_ENGINE_API_C Mision_Masacre* CreateMision_Masacre(ScriptToInspectorInterface* script)
{
	Mision_Masacre* classInstance = new Mision_Masacre();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragInt("NumberOfEnemiesInLevel", &classInstance->enemiessize);
	script->AddDragBoxGameObject("Player Storage", &classInstance->playerStorageGO);

	return classInstance;
}

void Mision_Masacre::Start()
{

	enemies.resize(enemiessize);
	misionCompleted = API_QuickSave::GetBool("Mision_Masacre");
	Game::FindGameObjectsWithTag("Enemy", &enemies[0], enemiessize); 
	 
	playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
	if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");

}
void Mision_Masacre::Update()
{
	if (misionCompleted == false)
	{
		for (int i = 0; i < enemiessize; i++)
		{
			if (enemies[i].IsActive())continue;

			if (enemies[i].IsActive() == false)
			{
				numOfDeadEnemies++;
				if (numOfDeadEnemies == enemiessize)
				{
					misionCompleted = true;

					playerStorage->skillPoints += 2;
					playerStorage->SaveData();

				}
			}
		}

		numOfDeadEnemies = 0;
	
	}
}