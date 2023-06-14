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
    script->AddDragInt("Duals Pull Size", &classInstance->dualsPullSize);
    script->AddDragInt("Normals Pull Size", &classInstance->normalsPullSize);
    script->AddDragInt("Ricochet Pull Size", &classInstance->ricochetPullSize);
    script->AddDragBoxPrefabResource("Duals Projectile Prefab", &classInstance->projectilePrefabDuals);
    script->AddDragBoxPrefabResource("Normals Projectile Prefab", &classInstance->projectilePrefabNormals);
    script->AddDragBoxPrefabResource("Ricochet Projectile Prefab", &classInstance->projectilePrefabRicochet);
    script->AddDragInt("Shotgun Bomb Pull Size", &classInstance->shotgunBombPullSize);
    script->AddDragBoxPrefabResource("Shotgun Bomb Prefab", &classInstance->shotgunBombPrefab);
    script->AddDragBoxPrefabResource("Check Ricochet Traget Prefab", &classInstance->checkRicochetTargetsPrefab);
    script->AddDragBoxTextureResource("Duals Projectile Texture", &classInstance->dualsTex);
    script->AddDragBoxTextureResource("Semi Projectile Texture", &classInstance->semiTex);
    script->AddDragBoxTextureResource("Auto Projectile Texture", &classInstance->autoTex);
    script->AddDragBoxTextureResource("Burst Projectile Texture", &classInstance->burstTex);
    script->AddDragBoxTextureResource("Shotgun Projectile Texture", &classInstance->shotgunTex);
    script->AddDragBoxTextureResource("Ricochet Projectile Texture", &classInstance->ricochetTex);
    script->AddCheckBox("Testing", &classInstance->testing);
    return classInstance;
}

void ProjectilePull::Start()
{
    srand(time(NULL));

    playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on ProjectilePull Script.");

    automaticLevel = API_QuickSave::GetInt("automatic_level");

    for (size_t i = 0; i < dualsPullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefabDuals, API_GameObject());
        Projectile* projectileScript = (Projectile*)newProjectile.GetScript("Projectile");
        if (projectileScript == nullptr)
            break;
        projectileScript->Start();
        projectileScript->pull = this;
        pullDuals.push_back(newProjectile);
    }

    for (size_t i = 0; i < normalsPullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefabNormals, API_GameObject());
        Projectile* projectileScript = (Projectile*)newProjectile.GetScript("Projectile");
        if (projectileScript == nullptr)
            break;
        projectileScript->Start();
        projectileScript->pull = this;
        pullNormals.push_back(newProjectile);
    }

    for (size_t i = 0; i < ricochetPullSize; i++)
    {
        API_GameObject newProjectile = Game::InstancePrefab(projectilePrefabRicochet, API_GameObject());
        Projectile* projectileScript = (Projectile*)newProjectile.GetScript("Projectile");
        if (projectileScript == nullptr)
            break;
        projectileScript->Start();
        projectileScript->pull = this;
        pullRicochet.push_back(newProjectile);
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

API_GameObject ProjectilePull::GetFirstInactiveDualProjectile()
{
    for (size_t i = 0; i < dualsPullSize; i++)
    {
        if (!pullDuals[i].IsActive()) return pullDuals[i];
    }

    return pullDuals[0];
}

API_GameObject ProjectilePull::GetFirstInactiveNormalProjectile()
{
    for (size_t i = 0; i < normalsPullSize; i++)
    {
        if (!pullNormals[i].IsActive()) return pullNormals[i];
    }

    return pullNormals[0];
}

API_GameObject ProjectilePull::GetFirstInactiveRicochetProjectile()
{
    for (size_t i = 0; i <ricochetPullSize; i++)
    {
        if (!pullRicochet[i].IsActive()) return pullRicochet[i];
    }

    return pullRicochet[0];
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

    for (size_t i = 0; i < dualsPullSize; i++)
    {
        if (pullDuals[i].IsActive())
            continue;

        pullDuals[i].GetParticleSystem().SetRotation(API_Vector3(0.0f, 0.0f, playerGO.GetTransform().GetLocalRotation().y + 90));

    }

    API_GameObject go = GetFirstInactiveDualProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(dualsTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });
    go.GetParticleSystem().Play();

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
    API_GameObject go = GetFirstInactiveNormalProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(semiTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });
    go.GetParticleSystem().Play();

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
    API_GameObject go = GetFirstInactiveNormalProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().Rotate(0, rotateY, 0);
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(semiTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });
    go.GetParticleSystem().Play();

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
    API_GameObject go = GetFirstInactiveNormalProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(autoTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });
    go.GetParticleSystem().Play();

    Projectile* projectile = (Projectile*)go.GetScript("Projectile");
    projectile->speed = projectileSpeed;
    projectile->damage = projectileDamage + (autoForce * 1.5f);
    projectile->resistanceDamage = projectileResistanceDamage + (autoForce * 1.5f);
    projectile->lifeTime = projectileLifetime;
    projectile->type = PROJECTILE_TYPE::AUTO;
    projectile->ignoreGO = 0;

    switch (automaticLevel)
    {
    case 0:
        if (autoForce <= 8.0f) autoForce += 0.1f;
        break;
    case 1:
        if (autoForce <= 8.0f) autoForce += 0.1f;
        break;
    case 2:
        if (autoForce <= 12.0f) autoForce += 0.15f;
        break;
    case 3:
        if (autoForce <= 12.0f) autoForce += 0.225f;
        break;
    default:
        break;
    }
    resetAuto = 0.2f;
}

void ProjectilePull::LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale)
{
    API_GameObject go = GetFirstInactiveNormalProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(burstTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });
    go.GetParticleSystem().Play();

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
    API_GameObject go = GetFirstInactiveNormalProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(shotgunTex);
    go.GetRigidBody().SetBoxScale({ 0.3f, 0.3f, 0.3f });
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
    projectile->type = PROJECTILE_TYPE::SHOTGUN;
    projectile->ignoreGO = 0;
}

void ProjectilePull::LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, uint ignoreGO)
{
    //API_GameObject go = GetFirstInactiveShotgunBomb();
    //go.SetActive(true);
    //go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    //go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    //go.GetTransform().SetScale(projectileScale);
    //go.GetMeshRenderer().SetActive(true);
    ////go.GetMeshRenderer().ChangeMesh(normalMesh);
    //go.GetParticleSystem().Play();

    //float offsetY = rand() % 360;
    //go.GetTransform().Rotate(0, offsetY, 0);

    //ShotgunBomb* bomb = (ShotgunBomb*)go.GetScript("ShotgunBomb");
    //bomb->lifeTime = projectileLifetime;
    //bomb->ResetExposion();
    //bomb->ignoreGO = ignoreGO;
}

void ProjectilePull::LauchProjectileFLAMETHROWER(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn)
{
    API_GameObject go = GetFirstInactiveNormalProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetMeshRenderer().SetActive(false);
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
    API_GameObject go = GetFirstInactiveRicochetProjectile();
    go.SetActive(true);
    go.GetTransform().SetPosition(shootingSpawn.GetGlobalPosition());
    go.GetTransform().SetRotation(playerGO.GetTransform().GetGlobalRotation());
    go.GetTransform().SetScale(projectileScale);
    go.GetMeshRenderer().SetActive(true);
    go.GetMaterialCompoennt().ChangeAlbedoTexture(ricochetTex);
    go.GetRigidBody().SetBoxScale({ 0.5f, 0.5f, 0.5f });
    go.GetParticleSystem().Play();

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