#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "EnemyTank.h"

class AllyEnemiesDetection : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionStay(API::API_RigidBody other) override;

	API_GameObject tankEnemy;
	EnemyTank* tankScript;
	float distanceToAlly;
};

