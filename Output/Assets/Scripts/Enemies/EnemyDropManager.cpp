#include "EnemyDropManager.h"
HELLO_ENGINE_API_C EnemyDropManager* CreateEnemyDropManager(ScriptToInspectorInterface* script)
{
    EnemyDropManager* classInstance = new EnemyDropManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Enemy Drop Rate", &classInstance->enemyDropRate);
    script->AddDragFloat("Upgraded Enemy Drop Rate", &classInstance->upgradedEnemyDropRate);
    script->AddDragFloat("Enemy Ammo Drop Rate", &classInstance->enemyAmmoDropRate);
    script->AddDragFloat("Enemy Aid Kit Drop Rate", &classInstance->enemyAidKitDropRate);
    script->AddDragFloat("Enemy Power Ups Drop Rate", &classInstance->enemyPowerUpsDropRate);
    script->AddDragFloat("Box Drop Rate", &classInstance->BoxDropRate);
    script->AddDragFloat("Box Ammo Drop Rate", &classInstance->BoxAmmoDropRate);
    script->AddDragFloat("Box Aid Kit Drop Rate", &classInstance->BoxAidKitDropRate);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    script->AddDragInt("Pull Size per Drop Type", &classInstance->pullSize);
    script->AddDragBoxMeshResource("Lase Ammo Drop Mesh", &classInstance->prefab0);
    script->AddDragBoxMeshResource("First Aid Kit Drop Mesh", &classInstance->prefab1);
    script->AddDragBoxMeshResource("Speed Power Up Drop Mesh", &classInstance->prefab2);
    script->AddDragBoxMeshResource("Firearte Power Up Drop Mesh", &classInstance->prefab3);
    script->AddDragBoxMeshResource("Shield Drop Mesh", &classInstance->prefab4);
    script->AddDragBoxMeshResource("Max Ammo Drop Mesh", &classInstance->prefab5);
    script->AddDragBoxMeshResource("Slow Time Drop Mesh", &classInstance->prefab6);
    return classInstance;
}

void EnemyDropManager::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("PlayerStats missing in EnemyDropManager Script.");

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab0, API_GameObject());
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 0;
        newDrop.SetActive(false);
        pull0.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab0, API_GameObject());
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 1;
        newDrop.SetActive(false);
        pull1.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab0, API_GameObject());
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 2;
        newDrop.SetActive(false);
        pull2.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab0, API_GameObject());
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 3;
        newDrop.SetActive(false);
        pull3.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab0, API_GameObject());
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 4;
        newDrop.SetActive(false);
        pull4.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab5, API_GameObject());
        EnemyDrop* enemyDrop = (EnemyDrop*)newDrop.AddScript("EnemyDrop");
        enemyDrop->dropIndex = 5;
        newDrop.SetActive(false);
        pull5.push_back(newDrop);
    }
    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newDrop = Game::InstancePrefab(prefab6, API_GameObject());
        pull6.push_back(newDrop);
    }
}

void EnemyDropManager::Update()
{

}

void EnemyDropManager::SpinDropRate(API_Vector3 position) // check if enemy drop things
{
    float dropRate = rand() % 100;
    float currentDropRate;
    if (playerStats && playerStats->armoryTreeLvl > 2) currentDropRate = upgradedEnemyDropRate;
    else currentDropRate = enemyDropRate;

    if (dropRate < currentDropRate)
    {
        API_GameObject go;
        float drop = rand() % 100;

        if (drop < enemyAmmoDropRate)
        {
            go = GetFirstInactiveObject(0);
        }
        else if (drop < enemyAmmoDropRate + enemyAidKitDropRate)
        {
            go = GetFirstInactiveObject(1);
        }
        else if (drop < enemyAmmoDropRate + enemyAidKitDropRate + enemyPowerUpsDropRate)
        {
            float powerUpDrop = rand() % 5;

            if (powerUpDrop < 1.0f)
            {
                go = GetFirstInactiveObject(2);
            }
            else if (powerUpDrop < 2.0f)
            {
                go = GetFirstInactiveObject(3);
            }
            else if (powerUpDrop < 3.0f)
            {
                go = GetFirstInactiveObject(4);
            }
            else if (powerUpDrop < 4.0f)
            {
                go = GetFirstInactiveObject(5);
            }
            else if (powerUpDrop < 5.0f)
            {
                go = GetFirstInactiveObject(6);
            }
        }

        go.SetActive(true);
        go.GetTransform().SetPosition(position);
    }
}

API_GameObject EnemyDropManager::GetFirstInactiveObject(int index)
{
    switch (index)
    {
    case 0:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull0[i].IsActive()) return pull0[i];
        }
        break;
    case 1:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull1[i].IsActive()) return pull1[i];
        }
        break;
    case 2:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull2[i].IsActive()) return pull2[i];
        }
        break;
    case 3:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull3[i].IsActive()) return pull3[i];
        }
        break;
    case 4:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull4[i].IsActive()) return pull4[i];
        }
        break;
    case 5:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull5[i].IsActive()) return pull5[i];
        }
        break;
    case 6:
        for (size_t i = 0; i < pullSize; i++)
        {
            if (!pull6[i].IsActive()) return pull6[i];
        }
        break;
    default:
        return pull0[0];
        break;
    }
}

void EnemyDropManager::BoxSpinDropRate(API_Vector3 position)
{
    float dropRate = rand() % 100;
    
    if (dropRate < BoxDropRate)
    {
        API_GameObject go;

        float drop = rand() % 100;

        if (drop < BoxAidKitDropRate) // 50% first aid kit
        {
            go = GetFirstInactiveObject(1);
        }
        else if (drop < BoxAmmoDropRate + BoxAidKitDropRate)  // 50% Normal Ammo
        {
            go = GetFirstInactiveObject(0);
        }

        go.SetActive(true);
        API_Vector3 newPos = { position.x, position.y + 1.0f, position.z };
        go.GetTransform().SetPosition(newPos);

    }
    else 
    {
        return;
    }
}
