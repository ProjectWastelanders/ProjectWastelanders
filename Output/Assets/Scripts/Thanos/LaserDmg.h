#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "ThanosAttacks.h"
#include "ThanosMovement.h"

#include "API/API.h"

class LaserDmg : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionStay(API::API_RigidBody other);

	void OnCollisionExit(API::API_RigidBody other);

	float dmg = 5.0f;
	float dmgCooldown = 0.0f;

	API_GameObject boss;
	ThanosAttacks* Tattack;
	ThanosMovement* Tmov;
};

