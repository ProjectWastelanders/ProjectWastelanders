#include "Meteor.h"
#include "ThanosAttacks.h"

HELLO_ENGINE_API_C Meteor* CreateMeteor(ScriptToInspectorInterface* script)
{
	Meteor* classInstance = new Meteor();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	
	return classInstance;
}

void Meteor::Start()
{
	gameObject.GetParticleSystem().Play();  
	gameObject.GetRigidBody().SetVelocity(API_Vector3(0, -0.01f, 0));
}
void Meteor::Update()
{
	if (isFireOn == true) {
		//fire
		fireCounter += Time::GetDeltaTime();
		gameObject.GetTransform().SetPosition(position);
		if (fireCounter > 3.0f) {
			isFireOn = false;
			fireCounter = 0.0f;
			gameObject.GetMeshRenderer().SetActive(true);
			gameObject.GetTransform().Translate(0, -10, 0);
		}
	}
}
void Meteor::OnCollisionEnter(API::API_RigidBody other) {

	std::string detectionTag = other.GetGameObject().GetTag();
	std::string detectionName = other.GetGameObject().GetName();

	if (detectionTag == "Floor" && isFireOn == false) {
		gameObject.GetMeshRenderer().SetActive(false);
		position = gameObject.GetTransform().GetGlobalPosition();
		isFireOn = true;
		if (audioHasCollided == false) {
			audioHasCollided = true;
			Audio::Event("thanos_meteor_impact");
		}
	}
	if (detectionName == "Player" && isFireOn == true) {
		PlayerStats* pStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
		pStats->TakeDamage(dmg, 0);
	}
}

void Meteor::OnCollisionExit(API::API_RigidBody other) {

	std::string detectionTag = other.GetGameObject().GetTag();
	std::string detectionName = other.GetGameObject().GetName();

	if (detectionTag == "Floor") {
		audioHasCollided = false;
	}
}