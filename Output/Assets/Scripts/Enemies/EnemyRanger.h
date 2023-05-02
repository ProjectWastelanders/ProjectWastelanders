#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "EnemyGun.h"
#include "API/API.h"
#include "Enemy.h"

class EnemyRanger : HelloBehavior
{
    enum class AnimationState
    {
        NONE,
        IDLE,
        WALK,
        RUN,
        SHOOT,
        HITTED,
        DIE,
    };

public:
    enum class States {
        TARGETING,
        WANDERING,
        ATTACKIG,
        DYING

    };
    void Start() override;
    void Update() override;

    void Seek(float vel, API_Vector3 tarPos, API_RigidBody rb);
    void Wander(float vel, API_Vector3 point, API_RigidBody rb);
    void Attacking(float vel, API_Vector3 tarPos, API_RigidBody rb);

    API_Vector3 NormalizeVec3(float x, float y, float z);

    float Lerp(float a, float b, float time);
    void HitAnimation();

    bool wander = false;
    bool targeting = false;

    float detectionDis = 300.0f;
    float lossingDis = 350.0f;
    //distance between player and enemy while the enemy is aiming the player
    float disPlayer = 200.0f;
    float disShoot = 250.0f;

    float cooldownPoint = 3.0f;
    float outTime = 3.0f;
    float hitOutTime = 2.0f;

    API_GameObject target;
    API_GameObject actionZone;

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
    uint aimAnim;
    uint hitAnim;
    uint dieAnim;

    EnemyGun* enemyGun = nullptr;
    API_GameObject gunObj;

    Enemy* enemy = nullptr;
    PlayerStats* targStats = nullptr;

    int gunType;
    bool _canWalk = false;

    // mission
    bool scriptedForQuest = false;
private:
    int _avalPoints = 0;
    float _movCooldown;//max time that can be outside the zone
    float _outCooldown;//max time that can be outside the zone
    float _hitOutCooldown;//

};

