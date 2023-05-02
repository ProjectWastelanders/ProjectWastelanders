#include "ThanosMeleeDmg.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"
HELLO_ENGINE_API_C ThanosMeleeDmg* CreateThanosMeleeDmg(ScriptToInspectorInterface* script)
{
	ThanosMeleeDmg* classInstance = new ThanosMeleeDmg();

	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	script->AddDragBoxGameObject("Player", &classInstance->player);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosMeleeDmg::Start()
{

}
void ThanosMeleeDmg::Update()
{

}

void ThanosMeleeDmg::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionName == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		PlayerMove* pMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");
        pStats->TakeDamage(meleeDmg, 0);

		API_Vector3 normalizedvector = boss.GetTransform().GetGlobalPosition() - player.GetTransform().GetGlobalPosition();
		float x = normalizedvector.x * normalizedvector.x;
		float y = 0;
		float z = normalizedvector.z * normalizedvector.z;
		float sum = x + y + z;
		API_Vector3 direction = { normalizedvector.x / sum, 0, normalizedvector.z / sum };
		//pMove->RecieveImpulse(-direction, 0.25f, 50);

    }
}