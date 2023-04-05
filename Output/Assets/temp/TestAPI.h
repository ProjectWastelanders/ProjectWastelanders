#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class TestAPI : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject gameObjects[10];
};

