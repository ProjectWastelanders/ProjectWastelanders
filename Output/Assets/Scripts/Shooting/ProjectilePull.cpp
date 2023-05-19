#include "ProjectilePull.h"
#include "../Player/PlayerStats.h"
#include "ShotgunBombExplosion.h"
#include "CheckRicochetTargets.h"
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
    script->AddDragBoxPrefabResource("Check Ricochet Traget Prefab", &classInstance->checkRicochetTargetsPrefab);
    script->AddDragBoxTextureResource("Duals Projectile Texture", &classInstance->dualsTex);
    script->AddDragBoxTextureResource("Semi Projectile Texture", &classInstance->semiTex);
    script->AddDragBoxTextureResource("Auto Projectile Texture", &classInstance->autoTex);
    script->AddDragBoxTextureResource("Burst Projectile Texture", &classInstance->burstTex);
    script->AddDragBoxTextureResource("Shotgun Projectile Texture", &classInstance->shotgunTex);
    script->AddDragBoxTextureResource("Ricochet Projectile Texture", &classInstance->ricochetTex);
    script->AddDragBoxMeshResource("Normal Projectile Mesh", &classInstance->normalMesh);
    script->AddDragBoxMeshResource("Ricochet Projectile Mesh", &classInstance->ricochetMesh);
    script->AddCheckBox("Testing", &classInstance->testing);
    return classInstance;
}

void ProjectilePull::Start()
{
    srand(time(NULL));

    playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on ProjectilePull Script.");

    automaticLevel = API_QuickSave::GetInt("automatic_level");

    for (size_t i = 0; i < pullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefab, API_GameObject());
        Projectile* projectileScript = (Projectile*)newProjectile.GetScript("Projectile");
        projectileScript->pull = this;
        pull.push_back(newProjectile);
    }

    for (size_t i = 0; i < shotgunBombPullSize; i++)
    {
        API_GameObject newBomb = Game::InstancePrefab(shotgunBombPrefab, API_GameObject());
        ShotgunBomb* bombScript = (ShotgunBomb*)newBomb.GetScript("ShotgunBomb");
        API_GameObject explosion[1];
        newBomb.GetChildren(explosion);
        bombScript->explosion = (ShotgunBombExplosion*)explosion->GetScript("ShotgunBombExplosion");
        bombScript->explosion->shotgunBomb = newBomb;
        shotgunBombPull.push_back(newBomb);
    }

    ricochetDetector = Game::InstancePrefab(checkRicochetTargetsPrefab, API_GameObject());
    ricochetTargets = (CheckRicochetTargets*)ricochetDetector.GetScript("CheckRicochetTargets");
    if (ricochetTargets == nullptr) Console::Log("Missing CheckRicochetTargets on ProjectilePull Script.");
}

void ProjectilePull::Update()
{
    float dt;
    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (resetAuto >= 0.0f && autoForce >= 0.0f)
    {
        resetAuto -= dt;
        if (resetAuto <= 0.0f)
        {
            resetAuto = 0.0f;
            autoForce -= dt * 3.0f;
        }
    }

    if (autoForce < 0.0f) autoForce = 0.0f;
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

void ProjectilePull::LauchProjectileNORMAL(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(dualsTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->action = projectileAction;
    projectile->type = PROJECTILE_TYPE::NONE;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(semiTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::SEMI;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSECONDARY_SEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float rotateY, uint ignoreGO)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().Rotate(0, rotateY, 0);
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(semiTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::SECONDARY_SEMI;
    projectile->ignoreGO = ignoreGO;
}

void ProjectilePull::LauchProjectileAUTO(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(autoTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage + (autoForce * 1.5f);
    projectile->resistanceDamage = projectileResistanceDamage + (autoForce * 4.8f);
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::AUTO;
    projectile->ignoreGO = 0;

    switch (automaticLevel)
    {
    case 0:
        if (autoForce <= 6.0f) autoForce += 0.1f;
        break;
    case 1:
        if (autoForce <= 6.0f) autoForce += 0.1f;
        break;
    case 2:
        if (autoForce <= 9.0f) autoForce += 0.15f;
        break;
    case 3:
        if (autoForce <= 9.0f) autoForce += 0.225f;
        break;
    default:
        break;
    }
    resetAuto = 0.2f;
}

void ProjectilePull::LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(burstTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::BURST;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSHOTGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float randomDirectionRange)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(shotgunTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });

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
    projectile->type = PROJECTILE_TYPE::SHOTGUN;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, uint ignoreGO)
{
    API_GameObject go = GetFirstInactiveShotgunBomb();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(normalMesh);

    float offsetY = rand() % 360;
    go.GetTransform().Rotate(0, offsetY, 0);

    ShotgunBomb* bomb = (ShotgunBomb*)go.GetScript("ShotgunBomb");
    bomb->lifeTime = projectileLifetime;
    bomb->ResetExposion();
    bomb->ignoreGO = ignoreGO;
}

void ProjectilePull::LauchProjectileFLAMETHROWER(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    //go.GetMeshRenderer().SetActive(false);
    go.GetRigidBody().SetBoxScale({ 0.1f, 0.1f, 0.1f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::FLAMETHROWER;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileRICOCHET(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMeshRenderer().ChangeMesh(ricochetMesh);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(ricochetTex);
    go.GetRigidBody().SetBoxScale({ 0.5f, 0.5f, 0.5f });

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage;
    projectile->resistanceDamage = projectileResistanceDamage;
    projectile->lifeTime = projectileLifetime;
    projectile->targetsHitted = 0;
    projectile->type = PROJECTILE_TYPE::RICOCHET;
    projectile->ignoreGO = 0;

    ricochetDetector.SetActive(true);
    ricochetTargets->atachedToGO = go;
}

API_Vector3 ProjectilePull::CheckTargetDirectionRICOCHET(API_Vector3 ricochetPos, uint& targetUID, uint ignoreGO)
{
    if (!ricochetTargets) return {1, 0, 1};
    API_GameObject go;
    do
    {
        go = ricochetTargets->GetRandomTarget();
    } while (ignoreGO == go.GetUID());
    std::string compareTag = go.GetTag();
    if (compareTag != "Enemy" && compareTag != "Boss" && compareTag != "Thanos") return { 1, 0, 1 };
    
    float angleY = atan2((double)ricochetPos.z - (double)go.GetTransform().GetGlobalPosition().z, -(double)ricochetPos.x + (double)go.GetTransform().GetGlobalPosition().x);
    targetUID = go.GetUID();
    return { 0, angleY * (180.0f / 3.141592653589793238463f) + 90, 0 };
}