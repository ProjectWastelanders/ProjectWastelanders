#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AttackingShip : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void TakeDamage(float damage);

	float maxHealth = 1000;
	float health;
	//void OnCollisionEnter(API::API_RigidBody other) override;
};

