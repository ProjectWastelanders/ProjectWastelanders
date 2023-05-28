#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class ChestManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    int GetRandomLoot(bool canGetGun);
    int GetGunBlueprint();
    int GetSpecialGun();

    API_GameObject chest1;
    API_GameObject chest2;
    API_GameObject chest3;
    API_GameObject chest4;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;

    bool specialGunAsigned;
    bool gunBlueprintAsigned;
    int remainingUpgradeBlueprints;

    bool skipChest1;
    bool skipChest2;
    bool skipChest3;
    bool skipChest4;

    bool check;
};

