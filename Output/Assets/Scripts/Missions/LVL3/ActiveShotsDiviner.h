#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "../../Quests/ReachTheSpaceship.h"

class ActiveShotsDiviner : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other) override;
	API_GameObject ship;
	ReachTheSpaceship* shipScript = nullptr;
	bool canHearShoot = false;
};

