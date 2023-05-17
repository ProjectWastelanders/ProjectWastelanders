#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "ThanosAttacks.h"

class SpinSword : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other);

	ThanosAttacks* Tattack;
	API_GameObject boss;

	float spinCooldown = 0.0f;
	bool justStartSpinOnce = false;

	enum class SPINSTATE {
		STARTSPINNING,
		SPINNING,
		STOPSPINNING,
		NOTSPINNING,
	};

	SPINSTATE spinState = SPINSTATE::NOTSPINNING;
};

