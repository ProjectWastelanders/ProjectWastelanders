#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStats;

class PlayerInCombat : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	std::vector<API_GameObject> enemies;
	int enemiesSize = 1;

	PlayerStats* playerStats;
};

