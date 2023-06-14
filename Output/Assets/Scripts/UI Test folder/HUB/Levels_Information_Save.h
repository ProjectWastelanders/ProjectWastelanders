#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIText.h"

class Levels_Information_Save : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIText casete_text;
	API_UIText bluePrint_text;
	API_UIText goldPrint_text;

	int levels = 0;

};

