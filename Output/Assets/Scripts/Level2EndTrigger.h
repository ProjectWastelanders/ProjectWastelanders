#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Level2EndTrigger : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API_RigidBody other);
};

