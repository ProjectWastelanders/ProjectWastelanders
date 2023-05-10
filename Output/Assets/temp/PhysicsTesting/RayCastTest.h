#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_RayCast.h"
#include "API/API_RayCastCollision.h"
#include "API/API_Vector3.h"
#include <vector>

class RayCastTest : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_RigidBody rigidBody;
};

