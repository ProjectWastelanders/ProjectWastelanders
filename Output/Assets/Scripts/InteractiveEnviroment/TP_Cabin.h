#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStats;
class PlayerBeingTargeted;
class TpRotateSphere;

class TP_Cabin : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void OnCollisionStay(API_RigidBody other);
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

	uint big_effectSpherePrefab;
	uint small_effectSpherePrefab;
	API_GameObject effectSphere;
	API_GameObject effectSphere2;
	API_GameObject childSphere;
	API_GameObject childSphere2;

	bool hasSpawnedSphere;
	float scaleValue_big;
	float scaleValue_small;

	float startParticles;
	float endParticles;
	float tpTime;
	float enableCanTp;

	bool canTp;
	TpRotateSphere* rotateSphere1;
	TpRotateSphere* rotateSphere2;

	bool isNotHolding = true;

};

