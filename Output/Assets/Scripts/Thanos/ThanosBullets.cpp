#include "ThanosBullets.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C ThanosBullets* CreateThanosBullets(ScriptToInspectorInterface* script)
{
	ThanosBullets* classInstance = new ThanosBullets();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ThanosBullets::Start()
{

}
void ThanosBullets::Update()
{

}

void ThanosBullets::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionName == "Player")
	{
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(20, 0);

	}
}