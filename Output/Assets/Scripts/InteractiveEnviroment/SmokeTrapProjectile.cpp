#include "SmokeTrapProjectile.h"
#include "../Player/PlayerStats.h"
#include "../Enemies/Enemy.h"
HELLO_ENGINE_API_C SmokeTrapProjectile* CreateSmokeTrapProjectile(ScriptToInspectorInterface* script)
{
	SmokeTrapProjectile* classInstance = new SmokeTrapProjectile();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void SmokeTrapProjectile::Start()
{

}

void SmokeTrapProjectile::Update()
{
    lifeTime -= Time::GetDeltaTime();

    if (lifeTime <= 0)
    {
        Destroy();
        return;
    }

    gameObject.GetTransform().Translate(gameObject.GetTransform().GetForward() * speed * Time::GetDeltaTime());
}

void SmokeTrapProjectile::Destroy()
{
    gameObject.GetTransform().SetPosition(0, 8999, 0);
    gameObject.SetActive(false);
}

void SmokeTrapProjectile::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Player")
    {
        PlayerStats* playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        playerStats->TakeDamage(10.0f, 10.0f);
        Destroy();
    }
    else if (detectionTag == "Enemy")
    {
        Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
        enemy->TakeDamage(10.0f, 10.0f);
        Destroy();
    }
}