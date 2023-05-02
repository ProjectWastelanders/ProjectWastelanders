#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HUD_SHIELD_BROKEN : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void BrokenShield_Animation(bool isbroken);

	API_GameObject object_to_move;
	int counter = 0;
	float movment_position = 0;
	float movment_repetitions = 0;
	float movment_speed = 0;
	float original_position = 0;
	float variable_position = 0;
	float original_positionY= 0;

	bool moviment_Dreta = true;

	bool breack_shield_boss_anim = false;
	bool breack_shield_Player_anim = false;
};

