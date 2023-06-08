#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    int enemiesAmount;
    API_GameObject enemies[99];

    void SaveEnemiesState(int levelIndex);
    void LoadEnemiesState(int levelIndex);
};

