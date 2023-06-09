#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "Enemy.h"
#include "EnemyGun.h"

class EnemyTank : HelloBehavior
{
public:

	enum class AnimStates {
		IDLE,
		WALK,
		DIE
	};

	enum class States {
		WANDERING,
		TARGETING,
		ATTACKING,
		DYING
	};

	void Recovering();

	void Start() override;
	void Update() override;

	float TakeDamageTank(float life, float damage);
	void DieTank();
	void CheckDistance();
	void ReturnToZone();
	void MoveToDirection(float pointX, float pointY, float velocity);
	void EscapeFromDirection(float pointX, float pointY, float velocity);

	void ProtectEnemy();
	//void OnCollisionEnter(API::API_RigidBody other) override;

	void Wander();
	void Seek();
	void Attack();

	void BlinkHealth();
	void BlinkShield();

	void FadeIn();
	void FadeOut();
	void FadeInShield();
	void FadeOutShield();
	float Lerp(float a, float b, float time);

	/*void Seek(float vel, API_Vector3 tarPos, API_RigidBody rb);
	void Attacking(float vel, API_Vector3 tarPos, API_RigidBody rb);*/

	float currentShield;
	float maxShield;
	float recoverShieldTime;

	float healthRestorePerSecond;
	float healthRestoreCooldown;

	//API_GameObject tank;
	Enemy* enemyScript;

	EnemyGun* enemyGun = nullptr;
	API_GameObject gunObj;
	int gunType;
	States state;

	float detectionDistance;
	/*float walkVelocity;
	float seekVelocity;*/
	API_Vector3 initialPosition;
	float initPosRange;
	API_GameObject target;

	API_GameObject actionZone;
	API_GameObject zone;
	API_RigidBody zoneRb;

	float attackRange;
	float approximateRange;
	float separateRange;
	float returnToZoneDistance;

	API_GameObject protectedEnemy;
	API_AnimationPlayer animationPlayer;
	float alliesDistance;

	bool isProtectingAlly;

	uint idleAnim;
	uint walkAnim;
	uint dieAnim;

	API_ShaderComponent material;
	float blinkingTimer;
	float nonBlinkingTimer;

	bool hasToBlinkHealing;
	bool hasToBlinkShield;

	API_ParticleSystem healParticles;

private:
	bool isRecoveringShield;
	bool isRestoringHealth;
	bool hasToRestoreHealth;
	float shieldRecoverCounter;
	float healthRestoreCounter;
	float targetDistance;
	bool isReturning;

	float nonBlinkingTime;
	float blinkingTime;

	bool isBlinking;

	AnimStates animState;

	float _fadeInCooldown_blink;
	float _fadeOutCooldown_blink;
	bool fading_blink;

	float _r = 0;
	float _g = 0;
	float _b = 0;

	float fadeInTime = 1.0f;
	float fadeOutTime = 1.0f;

	float r_blink = 2;
	float g_blink = 2;
	float b_blink = 2;


	float blinkingTimer_Shield;
	float blinkingTime_Shield;

	float fadeInTime_Shield = 1.0f;
	float fadeOutTime_Shield = 1.0f;

	//float isBlinking_Shield;
	float fading_blink_shield;
	bool isFirstHitFrame;
};

