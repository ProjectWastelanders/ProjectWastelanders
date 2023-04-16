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

    API_GameObject GetFirstInactiveProjectile();
    API_GameObject GetFirstInactiveShotgunBomb();
    API_GameObject GetFirstInactiveElectricityChain();
    uint GetFirstEmptyElectricityChainExeption();
    void LauchProjectileNORMAL(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, PROJECTILE_ACTION projectileAction);
    void LauchProjectileSEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileSECONDARY_SEMI(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float rotateY);
    void LauchProjectileAUTO(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileBURST(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileSHOTGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale, float randomDirectionRange);
    void LauchProjectileSHOTGUN_BOMB(float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchProjectileHANDGUN(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    void LauchELECTRICITY_CHAIN(float delay, float damage, float resistanceDamage, API_GameObject attachedToGO, uint exceptionsVectorIndex);
    void LauchProjectileFLAMETHROWER(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn);
    void LauchProjectileRICOCHET(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, API_Vector3 projectileScale);
    API_Vector3 CheckTargetDirectionRICOCHET(API_Vector3 ricochetPos);

    int pullSize;
    std::vector<API_GameObject> pull;
    // shotgun
    int shotgunBombPullSize;
    std::vector<API_GameObject> shotgunBombPull;
    // handgun
    int electricityChainPullSize;
    std::vector<API_GameObject> electricityChainPull;
    std::vector<std::vector<API_GameObject>> electricityChainExeptions;
    std::vector<int> electricityChainExeptionsAmountActive;
    // ricochet
    API_GameObject ricochetDetector;
    CheckRicochetTargets* ricochetTargets;
    
    API_GameObject playerGO;
    PlayerStats* playerStats;
    uint projectilePrefab;
    uint shotgunBombPrefab;
    uint electricityChainPrefab;
    uint checkRicochetTargetsPrefab;

    float autoForce = 0.0f;
};