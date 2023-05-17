#include "PlayerGun.h"
HELLO_ENGINE_API_C PlayerGun* CreatePlayerGun(ScriptToInspectorInterface* script)
{
    PlayerGun* classInstance = new PlayerGun();
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
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Ammo Type", &classInstance->ammoType);
    script->AddInputBox("Audio Event String", &classInstance->audioEventString);
    return classInstance;
}

void PlayerGun::Start()
{

}

void PlayerGun::Update()
{

}

void PlayerGun::Shoot()
{

}

void PlayerGun::EnableGuns(bool enable)
{
    
}

void PlayerGun::SetGunStatsPerLevel(int level)
{

}

void PlayerGun::LauchProjectile(API_Transform projectileSpawn, PROJECTILE_TYPE type, PROJECTILE_ACTION projectileAction, float randomDirectionRange)
{
    ProjectilePull* pull = (ProjectilePull*)projectilePull.GetScript("ProjectilePull");
    if (pull == nullptr)
    {
        Console::Log("ProjectilePull not asigned");
        return;
    }

    switch (type)
    {
    case PROJECTILE_TYPE::NONE:
        pull->LauchProjectileNORMAL(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileScale, projectileAction);
        break;
    case PROJECTILE_TYPE::SEMI:
        pull->LauchProjectileSEMI(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileScale);
        break;
    case PROJECTILE_TYPE::AUTO:
        pull->LauchProjectileAUTO(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileScale);
        break;
    case PROJECTILE_TYPE::BURST:
        pull->LauchProjectileBURST(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileScale);
        break;
    case PROJECTILE_TYPE::SHOTGUN:
        pull->LauchProjectileSHOTGUN(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileScale, randomDirectionRange);
        break;
    case PROJECTILE_TYPE::FLAMETHROWER:
        pull->LauchProjectileFLAMETHROWER(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn);
        break;
    case PROJECTILE_TYPE::RICOCHET:
        pull->LauchProjectileRICOCHET(projectileSpeed, projectileDamage, projectileResistanceDamage, projectileLifetime, projectileSpawn, projectileScale);
        break;
    default:
        break;
    }
}

void PlayerGun::PlayShotSound(API_String eventString)
{
    Audio::Event(eventString.c_str());
}