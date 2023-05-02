#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SpaceShip : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_MeshRenderer mesh1;
	API_MeshRenderer mesh2;

	API_Transform point1;
	API_Transform point2;
	float speed = 2.0f;
	API_Vector3 direction;

	float cd;
	bool moving = false;
};

