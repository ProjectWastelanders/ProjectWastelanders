#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class Map : HelloBehavior
{
public:
    void Start() override;
    void Update() override;
    void MissionsEnable(bool hasMap);

    API_GameObject casette1, casette2, casette3;
    API_GameObject mapGO;
    API_GameObject playerStorageGO;

    PlayerStorage* playerStorage;

    int levelIndex;

    bool mapOpen = false;
};

