#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "Player/PlayerStats.h"

#include "API/API.h"

class HUD_Ultimate : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void HUD_Ultimate_Active(float IsActive);
	//
	API_UIImage hudActive;
	API_GameObject playerUltimate_GO;
	PlayerStats* playerUltimate;
	//API_GameObject HudDisactive;
	//
	//bool UltimateAvaliable = false;

};

