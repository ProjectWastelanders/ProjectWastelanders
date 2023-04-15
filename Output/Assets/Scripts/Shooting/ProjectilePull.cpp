#include "ProjectilePull.h"
#include "../Player/PlayerStats.h"
#include "ShotgunBombExplosion.h"
#include "ElectricityChain.h"
#include <time.h>
HELLO_ENGINE_API_C ProjectilePull* CreateProjectilePull(ScriptToInspectorInterface* script)
{
    ProjectilePull* classInstance = new ProjectilePull();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    script->AddDragInt("Pull Size", &classInstance->pullSize);
    script->AddDragBoxPrefabResource("Projectile Prefab", &classInstance->projectilePrefab);
    script->AddDragInt("Shotgun Bomb Pull Size", &classInstance->shotgunBombPullSize);
    script->AddDragBoxPrefabResource("Shotgun Bomb Prefab", &classInstance->shotgunBombPrefab);
    script->AddDragInt("Electricity Chain Pull Size", &classInstance->electricityChainPullSize);
    script->AddDragBoxPrefabResource("Electricity Chain Prefab", &classInstance->electricityChainPrefab);
    return classInstance;
}

void ProjectilePull::Start()
{
    srand(time(NULL));

    playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on ProjectilePull Script.");

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefab, API_GameObject());
        //all start
        Projectile* projectileScript = (Projectile*)newProjectile.GetScript("Projectile");
        projectileScript->pull = this;
        pull.push_back(newProjectile);
    }

    for (size_t i = 0; i < shotgunBombPullSize; i++)
    {
        API_GameObject newBomb = Game::InstancePrefab(shotgunBombPrefab, API_GameObject());
        //all start
        ShotgunBomb* bombScript = (ShotgunBomb*)newBomb.GetScript("ShotgunBomb");
        API_GameObject explosion[1];
        newBomb.GetChildren(explosion);
        bombScript->explosion = (ShotgunBombExplosion*)explosion->GetScript("ShotgunBombExplosion");
        bombScript->explosion->shotgunBomb = newBomb;
        shotgunBombPull.push_back(newBomb);
    }

    for (size_t i = 0; i < electricityChainPullSize; i++)
    {
        API_GameObject newChain = Game::InstancePrefab(electricityChainPrefab, API_GameObject());
        //all start
        ElectricityChain* electricityChain = (ElectricityChain*)newChain.GetScript("ElectricityChain");
        electricityChain->pull = this;
        electricityChainPull.push_back(newChain);
    }
}

void ProjectilePull::Update()
{
    float dt;
    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (autoForce > 0.0f)
    {
        autoForce -= dt * 0.1f;
        if (autoForce <= 0.0f) autoForce = 0.0f;
    }
}

API_GameObject ProjectilePull::GetFirstInactiveProjectile()
{
    for (size_t i = 0; i < pullSize; i++)
    {
        if (!pull[i].IsActive()) return pull[i];
    }

    return pull[0];
}

API_GameObject ProjectilePull::GetFirstInactiveShotgunBomb()
{
    for (size_t i = 0; i < shotgunBombPullSize; i++)
    {
        if (!shotgunBombPull[i].IsActive()) return shotgunBombPull[i];
    }

    return shotgunBombPull[0];
}

API_GameObject ProjectilePull::GetFirstInactiveElectricityChain()
{
    for (size_t i = 0; i < electricityChainPullSize; i++)
    {
        if (!electricityChainPull[i].IsActive()) return electricityChainPull[i];
    }

    return electricityChainPull[0];
}

void ProjectilePull::LauchProjectileNORMAL(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::SEMI;
}

void ProjectilePull::LauchProjectileSEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::SEMI;
}

void ProjectilePull::LauchProjectileSECONDARY_SEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction, float rotateY)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().Rotate(0, rotateY, 0);
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::SECONDARY_SEMI;
}

void ProjectilePull::LauchProjectileAUTO(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage + (autoForce * 15.8f);
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::AUTO;

    if (autoForce <= 6.0f) autoForce += 0.3f;

    Console::Log(std::to_string(projectile->damage));
}

void ProjectilePull::LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::BURST;
}

void ProjectilePull::LauchProjectileSHOTGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction, float randomDirectionRange)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    if (randomDirectionRange > 0.0f)
    {
        float offsetX = (-49 + rand() % (100)) * randomDirectionRange;
        float offsetY = (-49 + rand() % (100)) * randomDirectionRange;
        go.GetTransform().Rotate(offsetX, offsetY, 0);
    }

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::SHOTGUN;
}

void ProjectilePull::LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveShotgunBomb();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    float offsetX = rand() % 360;
    float offsetY = rand() % 360;
    float offsetZ = rand() % 360;
    go.GetTransform().Rotate(offsetX, offsetY, offsetZ);

    ShotgunBomb* bomb = (ShotgunBomb*)go.GetScript("ShotgunBomb");
    bomb->lifeTime = projectileLifetime;
    bomb->shotgunBombTimer = 0.5f;
}

void ProjectilePull::LauchProjectileHANDGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::HANDGUN;
}

void ProjectilePull::LauchProjectileELECTRICITY_CHAIN(float delay, float damage, float resistanceDamage, API_GameObject attachedToGO)
{
    API_GameObject go = GetFirstInactiveElectricityChain();
    go.SetActive(true);
    go.GetParticleSystem().Play();

    ElectricityChain* electricityChain = (ElectricityChain*)go.GetScript("ElectricityChain");
    electricityChain->delay = delay;
    electricityChain->damage = damage;
    electricityChain->resistanceDamage = resistanceDamage;
    electricityChain->atachedToGO = attachedToGO;
    electricityChain->triggerActive = false;
    electricityChain->chainCount = 0;
}