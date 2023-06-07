#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Enemies/Enemy.h"
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

    Enemy* enemyScript[4];
    /*Enemy* enemyScript1;
    Enemy* enemyScript2;
    Enemy* enemyScript3;
    Enemy* enemyScript4;*/
    bool uwu = false;

    float cooldownHit=0.0f;
    float timeHit = 3.0f;

    API_Vector3 spawnPosShip[4];
};

