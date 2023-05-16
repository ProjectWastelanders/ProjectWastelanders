#include "PlayerShotgun.h"
HELLO_ENGINE_API_C PlayerShotgun* CreatePlayerShotgun(ScriptToInspectorInterface* script)
{
    PlayerShotgun* classInstance = new PlayerShotgun();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Projectiles per second", &classInstance->cadence);
    script->AddDragFloat("Extra % firerate", &classInstance->upgradeFireratePercentage);
    script->AddDragInt("Pellets per shot", &classInstance->pellets);
    script->AddDragFloat("Pellets Dispersion", &classInstance->pelletsDisersion);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerShotgun::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");

    //SetGunStatsPerLevel(API_QuickSave::GetInt("shotgun_level")); // read from save file

    if (cadence == 0)
    {
        fullShotCooldown = 0;
        fullShotCooldownWithPowerUp = 0;
    }
    else
    {
        fullShotCooldown = 1 / cadence;
        fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase

        if (playerStats->armoryTreeLvl > 1)
        {
            fullShotCooldown = 1 / (cadence + cadence * upgradeFireratePercentage / 100.0f);
            fullShotCooldownWithPowerUp = 1 / ((cadence + cadence * upgradeFireratePercentage / 100.0f) * 1.5f); // 50% increase
        }
    }
}

void PlayerShotgun::Update()
{
    float dt;
    if (playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (shotBuffer)
    {
        shotBufferCooldown -= Time::GetRealTimeDeltaTime();
        if (shotBufferCooldown <= 0)
        {
            shotBuffer = false;
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

void PlayerShotgun::Shoot()
{
    if (canShoot)
    {
        for (size_t i = 0; i < pellets; i++)
        {
            LauchProjectile(shootingSpawn, PROJECTILE_TYPE::SHOTGUN, PROJECTILE_ACTION::FLINCH, pelletsDisersion);
        }
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        playerStats->UseAmmo(ammoType);
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = SHOT_BUFFER;
    }
}

void PlayerShotgun::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
    shotBuffer = false;
}

void PlayerShotgun::SetGunStatsPerLevel(int level)
{
    switch (level)
    {
    case 0:
        projectileSpeed = 40.0f;
        projectileDamage = 5.0f;
        projectileResistanceDamage = 5.0f;
        projectileLifetime = 0.2f;
        cadence = 1.3f;
        pellets = 3;
        pelletsDisersion = 0.2f;
        // explosion damage 10
        // bomb num 2
        break;
    case 1:
        projectileSpeed = 40.0f;
        projectileDamage = 5.0f;
        projectileResistanceDamage = 5.0f;
        projectileLifetime = 0.2f;
        cadence = 1.3f;
        pellets = 4;
        pelletsDisersion = 0.2f;
        // explosion damage 10
        // bomb num 2
        break;
    case 2:
        projectileSpeed = 40.0f;
        projectileDamage = 5.0f;
        projectileResistanceDamage = 5.0f;
        projectileLifetime = 0.2f;
        cadence = 1.3f;
        pellets = 4;
        pelletsDisersion = 0.2f;
        // explosion damage 10
        // bomb num 3
        break;
    case 3:
        projectileSpeed = 40.0f;
        projectileDamage = 10.0f;
        projectileResistanceDamage = 10.0f;
        projectileLifetime = 0.2f;
        cadence = 1.3f;
        pellets = 4;
        pelletsDisersion = 0.2f;
        // explosion damage 15
        // bomb num 3
        break;
    default:
        Console::Log("Shotgun gun level can't be different from 0, 1, 2 or 3.");
        break;
    }
}