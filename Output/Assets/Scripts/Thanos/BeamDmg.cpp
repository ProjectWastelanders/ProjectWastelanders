#include "BeamDmg.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C BeamDmg* CreateBeamDmg(ScriptToInspectorInterface* script)
{
	BeamDmg* classInstance = new BeamDmg();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void BeamDmg::Start()
{

}
void BeamDmg::Update()
{

}

void BeamDmg::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionName = other.GetGameObject().GetName();
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionName == "Player")
	{
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(15, 0);

	}
}