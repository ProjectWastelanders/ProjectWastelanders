#include "PlayerFlamethrower.h"
HELLO_ENGINE_API_C PlayerFlamethrower* CreatePlayerFlamethrower(ScriptToInspectorInterface* script)
{
    PlayerFlamethrower* classInstance = new PlayerFlamethrower();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Projectile Pull", &classInstance->projectilePull);
    script->AddDragFloat("Projectile Speed", &classInstance->projectileSpeed);
    script->AddDragFloat("Projectile Damage", &classInstance->projectileDamage);
    script->AddDragFloat("Projectile Resistance Damage", &classInstance->projectileResistanceDamage);
    script->AddDragFloat("Projectile Lifetime", &classInstance->projectileLifetime);
    script->AddDragBoxTransform("Projectile Spawn", &classInstance->shootingSpawn);
    script->AddDragBoxParticleSystem("Fire Particles", &classInstance->fireParticles);
    script->AddDragFloat("Projectile ScaleX", &classInstance->projectileScale.x);
    script->AddDragFloat("Projectile ScaleY", &classInstance->projectileScale.y);
    script->AddDragFloat("Projectile ScaleZ", &classInstance->projectileScale.z);
    script->AddDragFloat("Ticks per second", &classInstance->cadence);
    script->AddDragFloat("Extra % firerate", &classInstance->upgradeFireratePercentage);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerFlamethrower::Start()
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

    playingParticlesCd = 0;
}

void PlayerFlamethrower::Update()
{
    if (playingParticlesCd > 0)
    {
        playingParticlesCd -= Time::GetDeltaTime();
        if (playingParticlesCd <= 0)
        {
            fireParticles.Stop();
            playingParticlesCd = 0.0f;
        }
    }

    if (canShoot) return;

    if (shotCooldown <= 0)
    {
        canShoot = true;
    }
    else
    {
        shotCooldown -= Time::GetDeltaTime();
    }
}

void PlayerFlamethrower::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn, PROJECTILE_ACTION::FLAMETROWER);
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        playerStats->UseAmmo(ammoType);


        if (playingParticlesCd <= 0) fireParticles.Play();
        playingParticlesCd = 0.2f;
    }
}

void PlayerFlamethrower::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
}