#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../PlayerGun.h"

class PlayerDuals : public PlayerGun
{
public:
    void Start() override;
    void Update() override;

    bool Shoot() override;
    void EnableGuns(bool enable) override;
    void SetGunStatsPerLevel(int level) override;

    void CalculateShoot(API_Transform projectileSpawn);
    float slowProbability;
    float freezeProbability;

    float fullShotCooldown;
    float shotCooldown;
    float fullShotCooldownWithPowerUp;
    bool canShoot = true;
    float shotBufferCooldown = SHOT_BUFFER;
    bool shotBuffer = false;
    float fullBurstDelay;
    float burstDelay;
    bool nextShot = false;

    API_Transform secondShootingSpawn;
    API_GameObject secondGun;
};

