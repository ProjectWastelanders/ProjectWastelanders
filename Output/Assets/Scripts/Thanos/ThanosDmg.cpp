#include "ThanosDmg.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C ThanosDmg* CreateThanosDmg(ScriptToInspectorInterface* script)
{
	ThanosDmg* classInstance = new ThanosDmg();
	script->AddDragFloat("Damage", &classInstance->dmg);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosDmg::Start()
{

}
void ThanosDmg::Update()
{

}

void ThanosDmg::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionName == "Player")
	{
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(dmg, 0);

	}
}