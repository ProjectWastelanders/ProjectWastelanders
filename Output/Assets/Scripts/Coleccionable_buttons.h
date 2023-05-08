#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Coleccionable_buttons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton button1;
	API_UIButton button2;
	API_UIButton button3;

};

