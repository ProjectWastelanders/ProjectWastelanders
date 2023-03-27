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
    script->AddDragBoxMeshResource("Projectile Mesh", &classInstance->projectileMesh);
    script->AddDragBoxTextureResource("Projectile Material", &classInstance->projectileMaterial);
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

    if (cadence != 0) fullShotCooldown = 1 / cadence;
    else fullShotCooldown = 0;

    if (playerStats->armoryTreeLvl > 1)
    {
        fullShotCooldown = fullShotCooldown + fullShotCooldown * upgradeFireratePercentage / 100.0f;
    }

    if (cadence != 0) fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase
    else fullShotCooldownWithPowerUp = 0;

    if (playerStats->armoryTreeLvl > 1)
    {
        fullShotCooldownWithPowerUp = fullShotCooldownWithPowerUp + fullShotCooldownWithPowerUp * upgradeFireratePercentage / 100.0f;
    }
}

void PlayerDuals::Update()
{
    if (shotBuffer)
    {
        shotBufferCooldown -= Time::GetDeltaTime();
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
            burstDelay -= Time::GetDeltaTime();
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
        shotCooldown -= Time::GetDeltaTime();
    }
}

void PlayerDuals::Shoot()
{
    if (canShoot)
    {
        CalculateShoot(shootingSpawn);
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        nextShot = true;
        burstDelay = fullBurstDelay;
    }
    else
    {
        shotBuffer = true;
        shotBufferCooldown = SHOT_BUFFER;
    }
}

void PlayerDuals::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
    secondGun.SetActive(enable);
    shotBuffer = false;
    nextShot = false;
}

void PlayerDuals::CalculateShoot(API_Transform projectileSpawn)
{
    if (playerStats->specialTreeLvl == 0) LauchProjectile(projectileSpawn);
    else if (playerStats->specialTreeLvl == 1)
    {
        float n = rand() % 100;
        if (n < slowProbability) LauchProjectile(projectileSpawn, PROJECTILE_ACTION::SLOW);
        else LauchProjectile(projectileSpawn);
    }
    else if (playerStats->specialTreeLvl == 2)
    {
        float n = rand() % 100;
        if (n < freezeProbability) LauchProjectile(projectileSpawn, PROJECTILE_ACTION::FREEZE);
        else LauchProjectile(projectileSpawn);
    }
}