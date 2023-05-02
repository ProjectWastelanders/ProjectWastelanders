#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ProjectilePull;

enum class PROJECTILE_ACTION
{
    NONE,
    SLOW,
    FREEZE,
    FLINCH
};

enum class PROJECTILE_TYPE
{
    NONE, // duals
    SEMI,
    SECONDARY_SEMI,
    AUTO,
    BURST,
    SHOTGUN,
    FLAMETHROWER,
    RICOCHET
};

class Projectile : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void Destroy();

    void OnCollisionEnter(API::API_RigidBody other);

    ProjectilePull* pull;

    float speed = 2.0f;
    float lifeTime = 5.0f;
    float damage = 0.0f;
    float resistanceDamage = 0.0f;
    PROJECTILE_ACTION action = PROJECTILE_ACTION::NONE;
    PROJECTILE_TYPE type = PROJECTILE_TYPE::NONE;

    uint ignoreGO = 0;

    bool reflected = false;

    // ricochet
    int targetsHitted = 0;
    uint ricochetTarget;

    int shotgunLevel;
};