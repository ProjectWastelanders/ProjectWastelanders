#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "Projectile.h"
#include "ShotgunBomb.h"

#define ELECTRICITY_DELAY 0.5f
#define ELECTRICITY_CHAIN_COUNT 3

class PlayerStats;
class CheckRicochetTargets;

class ProjectilePull : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject GetFirstInactiveDualProjectile();
    API_GameObject GetFirstInactiveNormalProjectile();
    API_GameObject GetFirstInactiveRicochetProjectile();
    API_GameObject GetFirstInactiveShotgunBomb();
    void LauchProjectileNORMAL(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileSEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileSECONDARY_SEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float rotateY, uint ignoreGO);
    void LauchProjectileAUTO(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileSHOTGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float randomDirectionRange);
    void LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, uint ignoreGO);
    void LauchProjectileFLAMETHROWER(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn);
    void LauchProjectileRICOCHET(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    API_Vector3 CheckTargetDirectionRICOCHET(API_Vector3 ricochetPos, uint& targetUID, uint ignoreGO);

    int dualsPullSize;
    int normalsPullSize;
    int ricochetPullSize;
    std::vector<API_GameObject> pullDuals;
    std::vector<API_GameObject> pullNormals;
    std::vector<API_GameObject> pullRicochet;
    // shotgun
    int shotgunBombPullSize;
    std::vector<API_GameObject> shotgunBombPull;
    // ricochet
    API_GameObject ricochetDetector;
    CheckRicochetTargets* ricochetTargets;
    
    API_GameObject playerGO;
    PlayerStats* playerStats;
    uint projectilePrefabDuals = 0;
    uint projectilePrefabNormals = 0;
    uint projectilePrefabRicochet = 0;
    uint shotgunBombPrefab;
    uint checkRicochetTargetsPrefab;

    float autoForce = 0.0f;
    float resetAuto;
    int automaticLevel;

    uint dualsTex = 0;
    uint semiTex = 0;
    uint autoTex = 0;
    uint burstTex = 0;
    uint shotgunTex = 0;
    uint ricochetTex = 0;

    bool testing = false;
};