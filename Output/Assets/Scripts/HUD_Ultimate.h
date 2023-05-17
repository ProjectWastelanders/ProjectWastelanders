#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class HUD_Ultimate : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void HUD_Ultimate_Active(bool IsActive);
	//
	//API_GameObject HudActive;
	//API_GameObject HudDisactive;
	//
	//bool UltimateAvaliable = false;

};

