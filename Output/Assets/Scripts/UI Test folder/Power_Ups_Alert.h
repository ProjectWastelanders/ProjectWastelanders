#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Power_Ups_Alert : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void Swap_PowerUp_Texture(int Power_Up);

	API_UIImage power_up_Alert;
	uint textures_Power_up[4];

	float timer_Active = 1.8;
	float opacity_Alert = 0;
	bool active = false;
};

