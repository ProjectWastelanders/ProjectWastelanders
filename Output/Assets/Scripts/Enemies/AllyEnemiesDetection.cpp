#include "AllyEnemiesDetection.h"

HELLO_ENGINE_API_C AllyEnemiesDetection* CreateAllyEnemiesDetection(ScriptToInspectorInterface* script)
{
	AllyEnemiesDetection* classInstance = new AllyEnemiesDetection();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Tank", &classInstance->tankEnemy);
	return classInstance;
}

void AllyEnemiesDetection::Start()
{
	distanceToAlly = gameObject.GetRigidBody().GetRadius();
	tankScript = (EnemyTank*)tankEnemy.GetScript("EnemyTank");
}
void AllyEnemiesDetection::Update()
{
	if (tankScript->isProtectingAlly == true) {
		float distanceToCurrentAlly = gameObject.GetTransform().GetGlobalPosition().Distance(tankScript->protectedEnemy.GetTransform().GetGlobalPosition());
		if (distanceToCurrentAlly > gameObject.GetRigidBody().GetRadius())
		{
			distanceToAlly = gameObject.GetRigidBody().GetRadius();
			tankScript->isProtectingAlly = false;
		}
	}
}

void AllyEnemiesDetection::OnCollisionStay(API::API_RigidBody other)
{
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionTag == "Enemy")
	{
		float distanceToCurrentAlly = gameObject.GetTransform().GetGlobalPosition().Distance(other.GetGameObject().GetTransform().GetGlobalPosition());
		if (distanceToCurrentAlly < distanceToAlly && tankScript != nullptr) {
			tankScript->protectedEnemy = other.GetGameObject();
			tankScript->isProtectingAlly = true;
			distanceToAlly = distanceToCurrentAlly;
		}
	}
}
