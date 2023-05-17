#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_Agent.h"
#include "EnemyDropManager.h"

class PlayerStats;

class Enemy : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void TakingDmgState();

    void TakeDamage(float damage, float resistanceDamage);
    void Die();

    void OnCollisionEnter(API::API_RigidBody other) override;

    void ActiveSlow(float q, float time);//q how slower the enemy will be
   
    void ActiveStun(float time);

    void EnemySlow(float xSlow, float time);//how slower the enemy will be
    void EnemyRecoil(float xRec);
    void EnemyStun(float timeStun);


    API_GameObject enemyDropManagerGO;
    EnemyDropManager* enemyDropManager;
    API_RigidBody enemyRb;
    API_ShaderComponent enemyShader;

    float maxHp = 100.0f;
    float currentHp;
    float minResistence = 10.0f;
    float maxResistance = 100.0f;
    float currentResistance;
    float speed = 75.0f;
    float acceleration = 1.2f;
    float currentSpeed = 0.0f;
    bool hasShield = false;
   // API_GameObject target;

    // Test Code, delete later
   // API_RigidBody rb;

    bool actSlow;
    bool actStun;

    bool targeting;
    bool meleeIsAtking = false;
    
    API_Vector3 baseRot;//base rotation object

    float stunVel;
    float slowVel;

    API_ParticleSystem hitParticles;

    bool isOut;
    bool isHit;
    bool isTargIn;
    bool takingDmg;
    bool dying;
    bool died;

    // bomb mechanic
    void AddBomb();
    void CheckBombs();
    int maxBombNum = 6;
    int currentBombNum = 0;
    API_GameObject bomb;
    uint textureBomb[6];
    int shotgunLevel;

    // burn
    void AddBurn();
    float burnTime = 0.0f;
    float resetBurn;

    // player reference
    PlayerStats* playerStats;
    float ultGain = 0;

    float _tAnimDie;
    float _coldAnimDie;

    float _tHitColor = 0.35f;
    API_Vector3 initialPos;

    API_Agent enemyAgent;
    bool isAgent;

private:

    float _coldStun;
    float _coldSlow;

    float _tSlow;
    float _qSlow;
    float _tStun;

    float _tAnimHit;
    float _coldAnimHit;
    
    float _coldHitColor;

    bool _hitShader;
};

