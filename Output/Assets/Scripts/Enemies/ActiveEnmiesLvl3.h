#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "../Quests/ReachTheSpaceship.h"
#include "Enemy.h"
class ActiveEnmiesLvl3 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void EnableEnemies();

	API_GameObject enebledEnemies[18];
	API_GameObject spaceShipGO;
	ReachTheSpaceship* spaceShip = nullptr;
	Enemy* enemyScript[18];
	bool actived = false;
	API_Vector3 spawnPosShip[18];
};

