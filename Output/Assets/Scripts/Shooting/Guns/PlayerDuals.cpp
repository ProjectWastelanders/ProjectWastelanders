#include "PlayerDuals.h"
HELLO_ENGINE_API_C PlayerDuals* CreatePlayerDuals(ScriptToInspectorInterface* script)
{
    PlayerDuals* classInstance = new PlayerDuals();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxTransform("Second Projectile Spawn", &classInstance->secondShootingSpawn);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragFloat("Extra % firerate", &classInstance->upgradeFireratePercentage);
    script->AddDragFloat("Burst Space", &classInstance->fullBurstDelay);
    script->AddDragBoxGameObject("Second Gun GO", &classInstance->secondGun);
    script->AddDragFloat("Slow %", &classInstance->slowProbability);
    script->AddDragFloat("Freeze %", &classInstance->freezeProbability);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerDuals::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    pull = (ProjectilePull*)projectilePull.GetScript("ProjectilePull");
    if (!pull->testing) SetGunStatsPerLevel(API_QuickSave::GetInt("duals_level")); // read from save file

    if (cadence == 0)
    {
        fullShotCooldown = 0;
        fullShotCooldownWithPowerUp = 0;
    }
    else
    {
        fullShotCooldown = 1 / cadence;
        fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase

        if (playerStats && playerStats->armoryTreeLvl > 1)
        {
            fullShotCooldown = 1 / (cadence + cadence * upgradeFireratePercentage / 100.0f);
            fullShotCooldownWithPowerUp = 1 / ((cadence + cadence * upgradeFireratePercentage / 100.0f) * 1.5f); // 50% increase
        }
    }
}

void PlayerDuals::Update()
{
    float dt;
    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (shotBuffer)
    {
        shotBufferCooldown -= Time::GetRealTimeDeltaTime();
        if (shotBufferCooldown <= 0)
        {
            shotBuffer = false;
        }
    }

    // burst
    if (nextShot)
    {
        if (burstDelay <= 0)
        {
            nextShot = false;
            CalculateShoot(secondShootingSpawn);
            PlayShotSound(audioEventString);
        }
        else
        {
            burstDelay -= dt;
        }
    }

    if (canShoot)
    {
        if (shotBuffer)
        {
            Shoot();
        }
        return;
    }

    if (shotCooldown <= 0)
    {
        canShoot = true;
    }
    else
    {
        shotCooldown -= dt;
    }
}

bool PlayerDuals::Shoot()
{
    if (canShoot)
    {
        CalculateShoot(shootingSpawn);
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats && playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        nextShot = true;
        burstDelay = fullBurstDelay;

        return true;
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = SHOT_BUFFER;

        return false;
    }
}

void PlayerDuals::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
    secondGun.SetActive(enable);
    shotBuffer = false;
    nextShot = false;
}

void PlayerDuals::SetGunStatsPerLevel(int level)
{
    switch (level)
    {
    case 0:
        projectileSpeed = 40.0f;
        projectileDamage = 20.0f;
        projectileResistanceDamage = 20.0f;
        projectileLifetime = 0.8f;
        cadence = 1.5f;
        fullBurstDelay = 0.1f;
        break;
    case 1:
        projectileSpeed = 40.0f;
        projectileDamage = 20.0f;
        projectileResistanceDamage = 20.0f;
        projectileLifetime = 0.8f;
        cadence = 2.0f;
        fullBurstDelay = 0.1f;
        break;
    case 2:
        projectileSpeed = 50.0f;
        projectileDamage = 20.0f;
        projectileResistanceDamage = 20.0f;
        projectileLifetime = 0.8f;
        cadence = 2.0f;
        fullBurstDelay = 0.1f;
        break;
    case 3:
        projectileSpeed = 50.0f;
        projectileDamage = 25.0f;
        projectileResistanceDamage = 20.0f;
        projectileLifetime = 0.8f;
        cadence = 2.0f;
        fullBurstDelay = 0.1f;
        break;
    default:
        Console::Log("Duals gun level can't be different from 0, 1, 2 or 3.");
        break;
    }
}

void PlayerDuals::CalculateShoot(API_Transform projectileSpawn)
{
    if (playerStats && playerStats->specialTreeLvl == 0) LauchProjectile(projectileSpawn, PROJECTILE_TYPE::NONE);
    else if (playerStats && playerStats->specialTreeLvl == 1)
    {
        float n = rand() % 100;
        if (n < slowProbability) LauchProjectile(projectileSpawn, PROJECTILE_TYPE::NONE, PROJECTILE_ACTION::SLOW);
        else LauchProjectile(projectileSpawn, PROJECTILE_TYPE::NONE);
    }
    else if (playerStats && playerStats->specialTreeLvl == 2)
    {
        float m = rand() % 100;
        float n = rand() % 100;
        if (m < freezeProbability) LauchProjectile(projectileSpawn, PROJECTILE_TYPE::NONE, PROJECTILE_ACTION::FREEZE);
        else if (n < slowProbability) LauchProjectile(projectileSpawn, PROJECTILE_TYPE::NONE, PROJECTILE_ACTION::SLOW);
        else LauchProjectile(projectileSpawn, PROJECTILE_TYPE::NONE);
    }
}