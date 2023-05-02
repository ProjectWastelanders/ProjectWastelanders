#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyMeleeAttackZone : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API::API_RigidBody other) override;
	void OnCollisionStay(API::API_RigidBody other) override;
	
	bool attack;
	bool shooted;
};

