#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "PlayerStats.h"

class PlayerUltimate : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionStay(API::API_RigidBody other) override;

    API_GameObject playerStatsGO;
    PlayerStats* playerStats;

    void UseUltimate();
    API_RigidBody ult;
    float bufferRJ;
    float bufferLJ;
    bool triggerUlt;
    bool endUlt = false;
};

