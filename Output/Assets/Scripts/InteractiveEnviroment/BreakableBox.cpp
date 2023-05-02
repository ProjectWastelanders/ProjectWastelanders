#include "BreakableBox.h"
#include "../Shooting/Projectile.h"
#include "../Player/PlayerMove.h"

HELLO_ENGINE_API_C BreakableBox* CreateBreakableBox(ScriptToInspectorInterface* script)
{
    BreakableBox* classInstance = new BreakableBox();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxParticleSystem("Box Destroyed Particle", &classInstance->boxDestroyed);
    script->AddDragInt("MaxHp", &classInstance->maxHp);
    script->AddDragBoxGameObject("Drop Manager", &classInstance->enemyDropManagerGO);

    return classInstance;
}

void BreakableBox::Start()
{

    currentHp = maxHp;

    enemyDropManager = (EnemyDropManager*)enemyDropManagerGO.GetScript("EnemyDropManager");
    if (enemyDropManager == nullptr)
    {
        Console::Log("Enemy Drop Manager is nullptr in Breakable Box");
    }
}
void BreakableBox::Update()
{

}

void BreakableBox::OnCollisionEnter(API_RigidBody other)
{
    if (!gameObject.IsActive())return;

    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Projectile")
    {

        Projectile* projectile = (Projectile*)other.GetGameObject().GetScript("Projectile");

        ShootBox(projectile->damage);

    }
    else if (detectionTag == "Player")
    {
        PlayerMove* playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");

        if (playerMove->isDashing)
        {
            DestroyBox();
        }
    }
}

void BreakableBox::ShootBox(float projectileDamage)
{

    // Health damage
    currentHp -= projectileDamage;
    if (currentHp <= 0)
    {
        currentHp = 0;
        DestroyBox();

        boxDestroyed.Play();

    }

}

void BreakableBox::DestroyBox()
{
    Audio::Event("box_breaking");

    if (enemyDropManager)
    {
        enemyDropManager->BoxSpinDropRate(gameObject.GetTransform().GetGlobalPosition());
    }

    gameObject.SetActive(false);
}

