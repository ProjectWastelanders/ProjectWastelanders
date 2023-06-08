#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ThanosCinematic;
class Jumper;
class ThanosLoop;

class JumperDeactivateOnThanos : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject thanosCinematicGO;
	ThanosCinematic* thanosCinematicRef;

	API_GameObject thanosLoopGO;
	ThanosLoop* thanosLoopRef;

	API_GameObject LightGO;

	API_GameObject jumper;
	Jumper* jumperRef;

	bool isDeactivated = false;
};

