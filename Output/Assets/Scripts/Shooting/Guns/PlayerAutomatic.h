#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../PlayerGun.h"

class PlayerAutomatic : public PlayerGun
{
public:
    void Start() override;
    void Update() override;

    bool Shoot() override;
    void EnableGuns(bool enable) override;
    void SetGunStatsPerLevel(int level) override;

    float fullShotCooldown;
    float shotCooldown;
    float fullShotCooldownWithPowerUp;
    bool canShoot = true;
};

