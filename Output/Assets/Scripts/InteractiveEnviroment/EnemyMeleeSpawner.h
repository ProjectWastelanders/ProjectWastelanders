#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Enemy;
class SpawnerArea;

class EnemyMeleeSpawner : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);

    //Spawn Related
    int GetEnemyIndexInactive();
    void SpawnEnemy(int i);
    void ShootSpawn(float damage);
    void DestroySpawn();

public:

    //Enemy Related
    API_GameObject enemy;
    API_GameObject target;
    API_GameObject actionZone;
    SpawnerArea* spawnerArea;
    API_GameObject parent;

    API_GameObject childs[60];

    std::vector<API_GameObject> enemiesInSpawn;

    API_RigidBody zoneRb;

    API_GameObject listPoints[5];

    uint enemyPrefabRes;

    API_ParticleSystem enemySpawnParticle;

    //Spawn Related

    int maxHp = 200;
    int currentHp;
    int spawnPoolSize;

    uint spawnDestroyedRes;

    API_GameObject spawnEntireRef;
    API_GameObject spawnDestroyedRef;
    API_GameObject parentOfSpawn;
    API_ParticleSystem spawnerDestroyedParticle;

    float spawnTimer = 0.0f;

    bool destroyed = false;

};

