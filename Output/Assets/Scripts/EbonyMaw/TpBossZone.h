#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BossLoop;

class TpBossZone : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_GameObject bossGO;
	BossLoop* bossloopref;

	API_GameObject colliderEntry;
	API_GameObject colliderExit;
};

