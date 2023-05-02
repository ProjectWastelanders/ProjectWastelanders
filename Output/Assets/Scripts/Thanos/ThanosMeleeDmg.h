#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ThanosMeleeDmg : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other);
	
	float meleeDmg = 30.0f;

	API_GameObject boss;
	API_GameObject player;
};

