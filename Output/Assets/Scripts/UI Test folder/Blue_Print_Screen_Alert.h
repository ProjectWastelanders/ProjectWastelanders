#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "API/API_UIText.h"
#include "API/API.h"

class Blue_Print_Screen_Alert : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void Swap_Texture(int Print);

	API_UIImage prints_Alert;
	uint textures[7];

	float timer_Active = 1.8;
	float opacity_Alert = 0;

	bool active = false;

};

