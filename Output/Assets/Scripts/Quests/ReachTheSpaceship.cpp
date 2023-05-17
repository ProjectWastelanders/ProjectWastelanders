#include "ReachTheSpaceship.h"
#include "../Player/PlayerStorage.h"
HELLO_ENGINE_API_C ReachTheSpaceship* CreateReachTheSpaceship(ScriptToInspectorInterface* script)
{
    ReachTheSpaceship* classInstance = new ReachTheSpaceship();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Final Text Panel", &classInstance->finalText);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("Current HP", &classInstance->currentHp);
    script->AddDragFloat("DamagePerProjectile", &classInstance->damagePerProjectile);
    script->AddDragBoxGameObject("Enemy to Active 0", &classInstance->enebledEnemies[0]);
    script->AddDragBoxGameObject("Enemy to Active 1", &classInstance->enebledEnemies[1]);
    script->AddDragBoxGameObject("Enemy to Active 2", &classInstance->enebledEnemies[2]);
    script->AddDragBoxGameObject("Enemy to Active 3", &classInstance->enebledEnemies[3]);
    script->AddDragBoxGameObject("Enemy to Active 4", &classInstance->enebledEnemies[4]);
    script->AddDragBoxGameObject("Enemy to Active 5", &classInstance->enebledEnemies[5]);
    script->AddDragBoxGameObject("Enemy to Active 6", &classInstance->enebledEnemies[6]);
    script->AddDragBoxGameObject("Enemy to Active 7", &classInstance->enebledEnemies[7]);
    return classInstance;
}

void ReachTheSpaceship::Start()
{
    currentHp = maxHp;
}

void ReachTheSpaceship::Update()
{
    Console::Log(std::to_string(currentHp));
}

void ReachTheSpaceship::OnCollisionEnter(API_RigidBody other)
{
    if (!enabled) return;

    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "EnemyProjectile")
    {
        currentHp -= damagePerProjectile;

        if (currentHp <= 0.0f)
        {
            //Mision Failed
            Scene::LoadScene("LoseMenu.HScene");
        }
    }
    else if (detectionTag == "Player")
    {
        if (playerStorage)
        {
            playerStorage->skillPoints += 3;
            if (currentHp >= maxHp / 2.0f) // secondary mision
            {
                playerStorage->skillPoints += 2;
            }
            playerStorage->SaveData();
        }
        API_QuickSave::SetBool("level3_completed", true);
        API_QuickSave::SetBool("IsInMiddleOfLevel", false);
        finalText.SetActive(true);
    }
}

void ReachTheSpaceship::EnableMision()
{
    enabled = true;

    for (size_t i = 0; i < 8; i++)
    {
        enebledEnemies[i].SetActive(true);
    }
}