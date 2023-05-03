#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class ReachTheSpaceship : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);

    void EnableMision();

    bool enabled = false;

    float maxHp;
    float currentHp;
    float damagePerProjectile;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;
    API_GameObject finalText;

    API_GameObject enebledEnemies[4];
};

