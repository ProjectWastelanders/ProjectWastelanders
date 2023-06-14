#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIImage.h"
#include "../Thanos/ThanosLoop.h"
#include "../Thanos/ThanosMovement.h"

#include "API/API.h"

class Thanos_HP_UI : HelloBehavior
{
public:
	void Start() override;
	void Update() override;

	void Thanos_Boss_Bar(float HP_Boss_Value /*int phase, bool Is_Active*/);

	API_GameObject  thanosStatsGO;
	ThanosLoop* thanosStats;	

	ThanosMovement* thanosMovment;

	//API_GameObject boss_shield_breack_Movment_GO;
	//HUD_SHIELD_BROKEN* boss_shield_breack_Movment;

	uint shield_thanos_texture[3];
	//uint shield_boss_texture;
	uint hp_thanos_texture;

	API_GameObject  thanos_HUD;
	API_GameObject  thanos_shield_icon;
	API_UIImage  thanos_hp_bar;
	API_UIImage  thanos_shield_bar;

	bool shield_Fight_On = false;
	int not_loop = 0;
};


