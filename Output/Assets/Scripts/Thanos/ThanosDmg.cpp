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
	gameObject.GetRigidBody().SetVelocity({ 0,0,0 });
}
void ThanosDmg::Update()
{
	gameObject.GetRigidBody().SetVelocity({ 0,0,0 });
}

void ThanosDmg::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();
	std::string detectionTag2 = gameObject.GetTag();
	if (detectionName == "Player")
	{
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		if(detectionTag2 == "ThanosBullet") gameObject.SetActive(false);

		pStats->TakeDamage(dmg, 0);

	}
	if (detectionTag == "Column" && detectionTag2 == "ThanosBullet") {
		Console::Log("laaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
		gameObject.SetActive(false);

	}
}