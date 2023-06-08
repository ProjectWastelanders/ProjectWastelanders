#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Columna : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	int numColumn;

	bool isDestroyed = false;
	bool isDestroying = false;

};

