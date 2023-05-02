#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_Agent.h"

class testNavAgent : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

private:

	API_Agent agent;
};

