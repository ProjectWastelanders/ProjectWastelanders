#include "ReachTheSpaceship.h"
#include "../Player/PlayerStorage.h"
HELLO_ENGINE_API_C ReachTheSpaceship* CreateReachTheSpaceship(ScriptToInspectorInterface* script)
{
    ReachTheSpaceship* classInstance = new ReachTheSpaceship();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Final Text Panel", &classInstance->finalText);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("DamagePerProjectile", &classInstance->damagePerProjectile);
    return classInstance;
}

void ReachTheSpaceship::Start()
{
    currentHp = maxHp;
}

void ReachTheSpaceship::Update()
{

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
}

void ReachTheSpaceship::OnCollisionStay(API_RigidBody other)
{
    if (!enabled) return;

    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
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
}