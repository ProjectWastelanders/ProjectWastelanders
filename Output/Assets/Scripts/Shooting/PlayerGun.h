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

    virtual void Shoot();
    virtual void EnableGuns(bool enable);
    virtual void SetGunStatsPerLevel(int level);

protected:
    void LauchProjectile(API_Transform projectileSpawn, PROJECTILE_ACTION projectileAction = PROJECTILE_ACTION::NONE, float randomDirectionRange = 0.0f);
    void PlayShotSound(std::string eventString);

public:
    API_GameObject player;
    PlayerStats* playerStats;

    API_GameObject projectilePull;

    float projectileSpeed;
    float projectileDamage;
    float projectileResistanceDamage;
    float projectileLifetime;
    API_Transform shootingSpawn;
    uint projectileMesh;
    uint projectileMaterial;
    API_Vector3 projectileScale;

    float cadence;
    float upgradeFireratePercentage;

    int ammoType;
    std::string audioEventString;

    bool testing;
};

