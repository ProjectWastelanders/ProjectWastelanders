#include "LaserDmg.h"
#include "../Player/PlayerStats.h"
#include "ThanosAttacks.h"
#include "ThanosMovement.h"
HELLO_ENGINE_API_C LaserDmg* CreateLaserDmg(ScriptToInspectorInterface* script)
{
	LaserDmg* classInstance = new LaserDmg();
	script->AddDragFloat("Dmg", &classInstance->dmg);
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void LaserDmg::Start()
{
	Tattack = (ThanosAttacks*)boss.GetScript("ThanosAttacks");
	Tmov = (ThanosMovement*)boss.GetScript("ThanosMovement");
}
void LaserDmg::Update()
{

}

void LaserDmg::OnCollisionStay(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	

	if (detectionName == "Player" && Tattack->thanosState == ThanosAttacks::THANOS_STATE::LASER)
	{

		dmgCooldown += Time::GetDeltaTime();

		if (dmgCooldown > 1.0f) {
			dmgCooldown = 0.0f;
		}
		if (dmgCooldown < 0.01f) {
			PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
			pStats->TakeDamage(dmg, 0);

		}

	}

}

void LaserDmg::OnCollisionExit(API::API_RigidBody other) {
	dmgCooldown = 0.0f;
}
