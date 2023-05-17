#include "Stone.h"
#include "RockDivider.h"
#include "../Player/PlayerStats.h"
#include "../EbonyMaw/BossAttacks.h"
//Pau Olmos

HELLO_ENGINE_API_C Stone* CreateStone(ScriptToInspectorInterface* script)
{
	Stone* classInstance = new Stone();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Stone::Start()
{
	bAttacks = (BossAttacks*)boss.GetScript("BossAttacks");
	
	gameObject.GetTransform().SetScale(gameObject.GetTransform().GetGlobalScale() / 2);

}
void Stone::Update()
{

}

void Stone::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();

	if (detectionName == "Player") {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(10, 0);
	}
}
