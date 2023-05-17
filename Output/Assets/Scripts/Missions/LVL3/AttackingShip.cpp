#include "AttackingShip.h"
HELLO_ENGINE_API_C AttackingShip* CreateAttackingShip(ScriptToInspectorInterface* script)
{
	AttackingShip* classInstance = new AttackingShip();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Max health", &classInstance->maxHealth);
	script->AddDragFloat("Current health", &classInstance->health);
	return classInstance;
}

void AttackingShip::Start()
{
	health = maxHealth;
}
void AttackingShip::Update()
{

	

}

void AttackingShip::TakeDamage(float damage)
{

	health -= damage;
	if (health <= 0)
	{
		health = 0;
		Scene::LoadScene("LoseMenu.HScene");
	}
}

//void AttackingShip::OnCollisionEnter(API::API_RigidBody other)
//{
//	std::string detectionTag = other.GetGameObject().GetTag();
//
//	if (detectionTag == "EnemyProjectile")
//	{
//		game
//	}
//
//}