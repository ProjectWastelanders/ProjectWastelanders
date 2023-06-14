#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../../Player/PlayerStats.h"

#include "API/API.h"

class Map_Swap_LVL3 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_Material map_LVL3;

	uint map1;
	uint map2;

	API_GameObject playerGO;
	//PlayerStats* playerStats;
};

