#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API.h"
#include "ThanosMeleeDmg.h"
#include "ThanosLoop.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

class ThanosAttacks : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void MeleeAttack();

	void DashAttack();

	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);
	void Seek2(API_GameObject* seeker, API_Vector3 target, float speed);
	void BulletSeek(API_GameObject* seeker, API_Vector3 target, float speed, int numBullet);
	void LookAt(API_Vector3 tarPos, API_GameObject* go);

	ThanosMeleeDmg* tMeleeDmg;
	bool isAttacking = false;
	bool dashing = false;

	API_GameObject boss;
	API_GameObject player;

	API_GameObject melee1;
	float meleeAttackTime = 0.0f;

	API_GameObject sword;
	API_GameObject bTarget;
	bool swordThrown = false;
	float swordSpeed = 1.0f;
	API_Vector3 aimPosition;
	float swordTime = 0.0f;

	float charge = 0.0f;

	API_GameObject defenseSword;

	float distSA = 0.0F;

	enum class THANOS_STATE {
		IDLE,
		SEEKING,
		MELEEATTACK,
		DASHATTACK,
		HOLDING,
		SPECIALATTACK,
		THROWINGATTACK,
		LASTSWORD,
		PULSE,
		BURST,
		BEAM,
		DASH2,
		LASER,
	};

	THANOS_STATE thanosState;

	API_Vector3 playerPosition;

	ThanosLoop* tLoop;
	PlayerStats* pStats;
	PlayerMove* pMove;
	bool finalSword = false;

	float explosionTime = 0.0f;
	API_GameObject explosionWave;

	bool explosionWave1HasArrived = false;
	bool explosionWave2HasArrived = false;


	API_GameObject bullet1;
	API_GameObject bullet2;
	API_GameObject bullet3;

	API_GameObject bullets[3];
	float bulletSpeed = 8.0f;

	bool bulletThrown[3] = { false,false,false };

	API_Vector3 playerPositions[3] = { 0,0,0 };
	float burstTimes[4] = { 0.01f,0.25f,0.5f,1.0f };
	float busrstTime = 0.0f;

	float attackType = 0.0f;

	API_GameObject beam1;
	API_GameObject beam2;
	API_GameObject beam3;
	API_GameObject beam4;

	API_GameObject beams[4];

	API_GameObject beamTarget1;
	API_GameObject beamTarget2;
	API_GameObject beamTarget3;
	API_GameObject beamTarget4;

	API_GameObject beamTargets[4];
	API_Vector3 beamPositions[4];

	float beamTime = 0.0f;
	float beamTimes[5] = { 0.31f,1.1f,1.11f,1.6f,3.5f };

	bool beamThrown[4] = { false,false,false,false };

	float beamSpeed = 0.75f;

	float Rotate(API_Vector3 target, float _angle, API_GameObject* rotator);
	float Rotate2(API_Vector3 target, float _angle, API_GameObject* rotator);

	float angle = 0.0f;

	float pulse = false;

	API_GameObject meteorRain;
	float meteorRainTime = 0.0f;
	float meteorRainSpeed = 5.0f;
	API_Vector3 meteorRainPosition = { 0,0,0 };
	float meteorRainCooldown = 0.0f;
	bool isRainingMeteors = false;
	void MeteorAttack();

	bool areaDmg = false;
	API_GameObject area;
	float dashSpeed = 1.0f;
	API_Vector3 thanosPosition;

	API_GameObject laserPosition;
	float laserSpeed = 1.5f;
	API_GameObject laserGO;
	API_ParticleSystem laserPS;

	API_GameObject meteors[30];
	API_Vector3 meteorsPosition[30];

	//ThanosAnimations
	API_AnimationPlayer thanosAnimationPlayer;
	uint thanosOutOfCombat;
	uint thanosWakeUp;
	uint thanosAimAnimation;
	uint thanosMeleeAnimation;
	uint thanosChargeAttackAnimation;
	uint thanosChargeOnlyAnimation;
	uint thanosGuantletRecoilAnimation;
	uint thanosIdleAnimation;
	uint thanosIdle2Animation;
	uint thanosMeteorAnimation;
	uint thanosPulseAnimation;
	uint thanosRunAnimation;
	uint thanosShootAnimation;
	uint thanosSpinAnimation;
	uint thanosStartSpinAnimation;
	uint thanosStopSpinAnimation;
	uint thanosStartSwordThrownAnimation;
	uint thanosStopSwordThrownAnimation;
	uint thanosSwordThrownAnimation;
	uint thanosWalkAnimation;
	uint thanosWalk2Animation;
};

