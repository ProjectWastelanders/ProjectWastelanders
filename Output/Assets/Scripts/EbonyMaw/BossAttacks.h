
#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "BossLoop.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"
#include "BossMovement.h"

class BossAttacks : HelloBehavior
{
public:
	void Start() override;
	void Update() override;
	void Seek(API_GameObject* seeker, API_Vector3 target, float speed, int rock, float endedAttacking, float time);
	void SelectRock();
	void HoldRock();
	void ReturnRock(API_GameObject* rock, int numRock, float endedAttacking);

	bool hasReachedTarget[15] = { false,false,false,false,false, false,false,false,false,false, false,false,false,false,false };

	API_GameObject rocks[20];
	API_Vector3 rockPositions[20];
	API_GameObject selectedRock;

	API_GameObject areaImpact;

	API_Vector3 dir[16] = { 0,0,0 };
	BossLoop* bLoop;
	PlayerStats* pStats;
	PlayerMove* pMove;
	API_GameObject boss;
	API_GameObject boss2;
	API_GameObject player;
	API_Vector3 playerPosition[15] = { 0,0,0 };
	API_Vector3 bossPosition[5] = { 0,0,0 };

	API_Vector3 lastPlayerPosition = { 0,0,0 };
	API_ParticleSystem groundFire;
	float fireTime = 0.0f;
	bool isFireOn = false;
	float returnFireRockTime;

	float timeAttack[7] = { 2.0f,4.0f,4.5f,5.0f,5.5f,6.0f,6.5f };
	float specialTimeAttack[11] = { 0.5f,1.0f,1.5f,2.0f,2.5f,3.0f,3.5f,4.0f,4.5f,5.0f,5.5f };
	float currentTimeAttack = 0.0f;
	bool endedAttacking = false;

	bool hasPlayerCoords = false;
	bool hasBossCoords = false;

	float xDistance = 0.0f;
	float yDistance = 0.0f;
	float zDistance = 0.0f;

	int difficultySetter = 0;
	int attackType = 0;
	int numRocks[4] = { 3,4,5,10 };

	int currentRock[16] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	bool selectRock = false;
	float speed = 1.0f;

	bool seeking = false;
	bool attacking = false;

	float throwing = false;
	float cooldownAttack = 0.0f;
	float preAttack = 0.0f;
	float dt = 0.0f;

	float rockDmg = 20.0f;
	float orbitingRockDmg = 40.0f;
	float fireDmg = 10.0f;
	float stoneDmg = 10.0f;

	float stoneSpeed = 1.0f;
	float stoneLifeTime = 0.5f;
	void SpecialAttack();
	void OrbitingRocks(API_GameObject* orbitingRock1, API_GameObject* orbitingRock2, API_GameObject* orbitingRock3, API_GameObject* orbitingRock4, float rotationSpeed, float radius);
	API_GameObject orbitingRocks;
	API_GameObject explosionWave1;
	float explosionRadius1 = 60.0f;
	float explosionRadius2 = 130.0f;
	API_GameObject explosionWave2;
	API_GameObject explosionWave3;
	float distSA;
	bool explosionWave1HasArrived = false;
	bool explosionWave2HasArrived = false;

	bool animAttack = false;

	float specialAttackCooldown = 30.0f;

	float explosionTime = 0.0f;

	float rotationSpeed = 1;
	float radius = 0.45f;

	bool exploted = false;

	float rotationBoss = 0.0f;

	enum class BOSS_STATE {
		KO,
		IDLE,
		SELECTATTACK,
		ROCKSELECT,
		SEEKING,
		HOLDING,
		THROWING,
		SPECIALATTACK,
		EXPLOSIONWAVE,
		FIREROCKATTACK,
		RESIZINGROCK,
	};

	BOSS_STATE bossState = BOSS_STATE::IDLE;

	float takeobjTimer;

	bool audioBigRock = false;
	bool audioRocks[5] = { false,false,false,false,false };
	bool audioRocksImpact[5] = { false,false,false,false,false };
	bool audioBoxes[10] = { false,false,false,false,false, false,false,false,false,false };

};
