#include "StickBomb.h"
#include "../Enemies/Enemy.h"
#include "../EbonyMaw/BossLoop.h"
HELLO_ENGINE_API_C StickBomb* CreateStickBomb(ScriptToInspectorInterface* script)
{
	StickBomb* classInstance = new StickBomb();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void StickBomb::Start()
{
	
}

void StickBomb::Update()
{
    if (destroy)
    {
        shotgunBomb.SetActive(false);
        destroy = false;
    }
}

void StickBomb::OnCollisionStay(API::API_RigidBody other)
{
    if (!triggerActive) return;

    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Enemy")
    {
        Enemy* enemy = (Enemy*)other.GetGameObject().GetScript("Enemy");
        if (enemy) enemy->TakeDamage(damage, resistanceDamage);
    }
    else if (detectionTag == "Boss")
    {
        BossLoop* miniBoss = (BossLoop*)other.GetGameObject().GetScript("BossLoop");
        if (miniBoss) miniBoss->TakeDamage(damage);
    }

    triggerActive = false;
    destroy = true;
}