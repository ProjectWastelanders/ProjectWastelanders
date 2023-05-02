#include "Meteor.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C Meteor* CreateMeteor(ScriptToInspectorInterface* script)
{
	Meteor* classInstance = new Meteor();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void Meteor::Start()
{

}
void Meteor::Update()
{

}

void Meteor::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionName == "Player")
    {
        PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        pStats->TakeDamage(meteorDmg, 0);
    }
}