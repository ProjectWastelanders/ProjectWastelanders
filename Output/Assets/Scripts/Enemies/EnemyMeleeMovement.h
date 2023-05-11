#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
//#include "Timer.hpp"
//#include <Timer.hpp>
#include "Enemy.h"
#include "../Player/PlayerStats.h"
#include "EnemyMeleeAttackZone.h"
#include "API/API.h"

class EnemyMeleeMovement : HelloBehavior
{
    enum class AnimationState
    {
        NONE,
        IDLE,
        WALK,
        RUN,
        CHARGE,
        ATTACK,
        DASH,
        HITTED,
        DIE
    };

public:
    enum class States {
        TARGETING,
        WANDERING,
        ATTACKIG,
        DASHING,
        DYING

    };

    void Start() override;
    void Update() override;

    void Seek(float vel, API_Vector3 tarPos, API_RigidBody rb);
    void Wander(float vel, API_Vector3 point, API_RigidBody rb);

    void WalkAway();
    void ChargeAttack();
    void Attack();

    void HitAnim();

    void LoookAt(API_Vector3 target);

    API_Vector3 NormalizeVec3(float x, float y, float z);

    float Lerp(float a, float b, float time);

    bool wander = false;
    bool targeting = false;
    bool dashing = false;
    // bool shooted = false;

    bool test = false;

    bool attacking = false;

    float detectionDis = 2.0f;
    float lossingDis = 10.0f;
    float lossingZoneDis = 200.0f;
    float rangeAtk = 60.0f;
    float outTime = 2.0f;
    float hitOutTime = 2.0f;

    float attackCD;
    float attackTime;
    float attackCharge;
    float attackCDCpy;
    float attackTimeCpy;
    float attackChargeCpy;
    float timer = 0.0f;

    float attackSpeed;
    float walkAwaySpeed;
    float chargeSpeed;

    float tDash;
    float velDash = 10;

    int probDash = 10;
    int sideDash = 0;
    API_Vector3 targetPosOnAttack;

    API_GameObject target; 
    API_GameObject actionZone;
    API_GameObject attackZoneGO;

    API_RigidBody zoneRb;

    API_GameObject listPoints[5];
    int numPoint = 0;

    API_Vector3 actualPoint;

    States enemState;

    // animation
    AnimationState animState;
    API_AnimationPlayer animationPlayer;
    uint idleAnim;
    uint walkAnim;
    uint runAnim;
    uint attackAnim;
    uint dashAnim;
    uint chargeAnim;
    uint dieAnim;
    uint hitAnim;


    Enemy* enemy = nullptr;
    PlayerStats* targStats = nullptr;
    EnemyMeleeAttackZone* attackZone = nullptr;
private:
    int _avalPoints = 0;
    float _outCooldown;//max time that can be outside the zone
    float _movCooldown;//
    float _hitOutCooldown;//
    float _dashCooldown;

    float _agTime;
    float _agCooldown;
    //Timer clock;
};




