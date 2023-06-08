#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "BossAttacks.h"

#include "API/API.h"

class RockDivider : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API::API_RigidBody other);

	std::vector<API_GameObject> stones;
	uint stoneMesh;

	API_GameObject boss;
	BossAttacks* bAttacks;
	bool rockDivided = false;

	int whichRockAmI = 0;
	
	float dt;
	float stoneTime = 0.0f;

	uint stoneTexture;
};

