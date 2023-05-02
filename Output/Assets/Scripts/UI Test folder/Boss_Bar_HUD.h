#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIImage.h"
#include "../EbonyMaw/BossLoop.h"
#include "../UI Test folder/HUD_SHIELD_BROKEN.h"

#include "API/API.h"

class Boss_Bar_HUD : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Boss_Bar(float HP_Boss_Value, float Shield_Boss_Value[3] /*float Shield_Boss_Value*/, int phase, bool Is_Active);

	API_GameObject bossStatsGO;
	BossLoop* bossStats;

	API_GameObject boss_shield_breack_Movment_GO;
	HUD_SHIELD_BROKEN* boss_shield_breack_Movment;

	uint shield_boss_texture[3];
	//uint shield_boss_texture;
	uint hp_boss_texture;

	API_GameObject boss_HUD;
	API_GameObject boss_shield_icon;
	API_UIImage boss_hp_bar;
	API_UIImage boss_shield_bar;

	bool shield_Boss_broked = false;
	int not_loop = 0;
};

