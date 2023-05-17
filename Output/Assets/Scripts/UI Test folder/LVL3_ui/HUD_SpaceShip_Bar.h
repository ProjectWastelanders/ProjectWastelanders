#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../../Quests/ReachTheSpaceship.h"
#include "API/API.h"

class HUD_SpaceShip_Bar : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void ShipBar(float hp);
	ReachTheSpaceship* ship = nullptr;
	API_GameObject shipGO;
	API_GameObject shipHUD;
	API_UIImage shipHpBar;
	uint hpShipTex;
};

