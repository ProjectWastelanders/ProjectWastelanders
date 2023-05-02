#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStats;
class PlayerBeingTargeted;

class TP_Cabin : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void OnCollisionStay(API_RigidBody other);

	API_Transform destination;
	API_ParticleSystem smoke;
	API_ParticleSystem destinationSmoke;

	API_GameObject playerGO;
	PlayerStats* playerStats;

	PlayerBeingTargeted* playerBeingTarget;

	float timeHoldButton = 0.0f;
};


