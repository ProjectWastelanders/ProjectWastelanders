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
	void OnCollisionEnter(API_RigidBody other);
	void OnCollisionExit(API_RigidBody other);
	void SpawnSphere();
	void DestroySphere();

	API_Transform destination;
	API_ParticleSystem smoke;
	API_ParticleSystem destinationSmoke;
	API_GameObject playerGO;
	PlayerStats* playerStats;

	PlayerBeingTargeted* playerBeingTarget;

	float timeHoldButton = 0.0f;
	float sphereGrowing = 0.0f;
	float sphereGrowingTime = 0.0f;

	uint effectSpherePrefab;
	API_GameObject effectSphere;
	API_GameObject effectSphere2;

	bool hasSpawnedSphere;
	float scaleValue;

	float startParticles;
	float endParticles;
	float tpTime;

	API_UIImage tpButton;

	bool canTp;
};

