#pragma once

#define SHOT_BUFFER 0.3f

#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "ProjectilePull.h"
#include "Projectile.h"
#include "../Player/PlayerStats.h"

class PlayerGun : protected HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    virtual bool Shoot();
    virtual void EnableGuns(bool enable);
    virtual void SetGunStatsPerLevel(int level);

protected:
    void LauchProjectile(API_Transform projectileSpawn, PROJECTILE_TYPE type, PROJECTILE_ACTION projectileAction = PROJECTILE_ACTION::NONE, float randomDirectionRange = 0.0f);
    void PlayShotSound(API_String eventString);

public:
    API_GameObject player;
    PlayerStats* playerStats;

    API_GameObject projectilePull;
    ProjectilePull* pull;

    float projectileSpeed;
    float projectileDamage;
    float projectileResistanceDamage;
    float projectileLifetime;
    API_Transform shootingSpawn;
    API_Vector3 projectileScale;

    float cadence;
    float upgradeFireratePercentage = 15.0f;

    int ammoType;
    API::API_String audioEventString;
};

