#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "EnemyDrop.h"
#include "../Player/PlayerStats.h"

class EnemyDropManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void SpinDropRate(API_Vector3 position);
    API_GameObject GetFirstInactiveObject(int index);

    void BoxSpinDropRate(API_Vector3 position);
    void BossSpinDropRate(API_Vector3 position);

    //Drop chances

    //Enemy
    float enemyDropRate;
    float upgradedEnemyDropRate;
    float enemyAmmoDropRate;
    float enemyAidKitDropRate;
    float enemyPowerUpsDropRate;
    API_GameObject playerStatsGO;
    PlayerStats* playerStats;

    //Breakable Box
    float BoxDropRate;
    float BoxAmmoDropRate;
    float BoxAidKitDropRate;

    // prefabs
    uint prefab0;
    uint prefab1;
    uint prefab2;
    uint prefab3;
    uint prefab4;
    uint prefab5;
    uint prefab6;

    // pull
    int pullSize;
    std::vector<API_GameObject> pull0;
    std::vector<API_GameObject> pull1;
    std::vector<API_GameObject> pull2;
    std::vector<API_GameObject> pull3;
    std::vector<API_GameObject> pull4;
    std::vector<API_GameObject> pull5;
    std::vector<API_GameObject> pull6;
};

