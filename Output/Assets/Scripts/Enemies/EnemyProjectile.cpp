#include "EnemyProjectile.h"
#include "../Player/PlayerStats.h"
#include "../Missions/LVL3/AttackingShip.h"
HELLO_ENGINE_API_C EnemyProjectile* CreateEnemyProjectile(ScriptToInspectorInterface* script)
{
    EnemyProjectile* classInstance = new EnemyProjectile();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void EnemyProjectile::Start()
{

}

void EnemyProjectile::Update()
{
    lifeTime -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }

    gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
}

void EnemyProjectile::Destroy()
{
    gameObject.GetParticleSystem().Stop();
    gameObject.SetActive(false);
}

void EnemyProjectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player" )
    {
        PlayerStats* playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        if (playerStats)
        {
            playerStats->TakeDamage(damage, resistanceDamage);
        }
        Destroy();

    }
    else if ( detectionTag == "Wall")
    {
        Destroy();
    }
    else if (detectionTag == "Milano")
    {
       // AttackingShip* shipScript = (AttackingShip*)other.GetGameObject().GetScript("AttackingShip");
        /*if (shipScript)
        {
            shipScript->TakeDamage(damage);
        }*/
        Destroy();

    }



}