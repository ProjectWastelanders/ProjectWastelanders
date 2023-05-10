#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#define ISLAND_NUM 2

class IslandMovement : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void MoveIslands();

	API_GameObject islands[ISLAND_NUM];
	API_Transform positions[ISLAND_NUM];
};

