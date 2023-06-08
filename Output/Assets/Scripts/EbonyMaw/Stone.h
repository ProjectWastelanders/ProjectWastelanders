#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "BossAttacks.h"

#include "API/API.h"

class Stone : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other);

	API_GameObject boss; 
	API_GameObject rock; 
	BossAttacks* bAttacks;

};

