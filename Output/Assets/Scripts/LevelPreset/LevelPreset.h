#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class LevelPreset : HelloBehavior
{
public:
	void Start() override;

	int lvlToSpawn;
};