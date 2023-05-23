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

    pull = (ProjectilePull*)projectilePull.GetScript("ProjectilePull");
    if (pull->testing) SetGunStatsPerLevel(0);

    if (cadence == 0)
    {
        fullShotCooldown = 0;
        fullShotCooldownWithPowerUp = 0;
    }
    else
    {
        fullShotCooldown = 1 / cadence;
        fullShotCooldownWithPowerUp = 1 / (cadence * 1.5f); // 50% increase
    }

    playingParticlesCd = 0;
}

void PlayerFlamethrower::Update()
{
    float dt;
    if (playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    fireParticles.SetInitialSpeed(player.GetTransform().GetForward() * 4.0f);

    if (playingParticlesCd > 0)
    {
        playingParticlesCd -= dt;
        if (playingParticlesCd <= 0)
        {
            fireParticles.StopEmitting();
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
        shotCooldown -= dt;
    }
}

void PlayerFlamethrower::Shoot()
{
    if (canShoot)
    {
        LauchProjectile(shootingSpawn, PROJECTILE_TYPE::FLAMETHROWER);
        PlayShotSound(audioEventString);
        canShoot = false;
        if (playerStats->fireratePowerUp) shotCooldown = fullShotCooldownWithPowerUp;
        else shotCooldown = fullShotCooldown;
        playerStats->UseAmmo(ammoType);

        if (playingParticlesCd <= 0.0f) fireParticles.Play();
        playingParticlesCd = 0.2f;
    }
}

void PlayerFlamethrower::EnableGuns(bool enable)
{
    gameObject.SetActive(enable);
}

void PlayerFlamethrower::SetGunStatsPerLevel(int level)
{
    projectileSpeed = 30.0f;
    projectileDamage = 10.0f;
    projectileResistanceDamage = 5.0f;
    projectileLifetime = 0.8f;
    cadence = 20.0f;
}