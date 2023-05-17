#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "ThanosAttacks.h"
#include "ThanosLoop.h"

#include "API/API.h"
class ThanosMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;


	float Rotate(API_Vector3 target, float _angle);
	void Seek(API_GameObject* seeker, API_Vector3 target, float speed);

	void Seek2(API_GameObject* seeker, API_Vector3 target, float speed);

	void Hide(API_GameObject* follower_position, API_Vector3 target_position, float speed);

	API_GameObject player;
	API_GameObject boss;

	float distBP;
	float bossSpeed = 0.75f;

	float angle = 0.0f;

	float inRange = false;
	float dashInRange = false;
	float dashCooldown = 0.0f;

	ThanosAttacks* Tattack;
	ThanosLoop* Tloop;

	float justOnce = false;

	bool isActive = false;

	float startFightTimer = 0.0f;
	bool isRunning = false;

	enum class WALKSTATE {
		NORMAL,
		BACKWARDS,
		IDLE,
	};
	float walkCooldown = 0.0f;
	WALKSTATE walkState = WALKSTATE::NORMAL;

	bool letsFight = false;
};

