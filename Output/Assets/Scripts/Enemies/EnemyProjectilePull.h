#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include"EnemyProjectile.h"
#include "API/API.h"

class EnemyProjectilePull : HelloBehavior
{
public:
    void Start() override;
    void Update() override;
    API_GameObject GetFirstActiveProjectile();
    void LauchProjectile(float projectileSpeed, float projectileDamage, float projectileResistanceDamage, float projectileLifetime, API_Transform shootingSpawn, uint projectileMesh, uint projectileMaterial, API_Vector3 enemyRotation, API_Vector3 projectileScale, bool randomDirection);

    int pullSize;
    std::vector<API_GameObject> pull;
    //API_ParticleSystem particleTest;
};

