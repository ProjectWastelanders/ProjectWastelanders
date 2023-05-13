#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../UI Test folder/HUD_BluePrints.h"
#include "API/API.h"

class PlayerIndicator;

class PlayerStorage : HelloBehavior
{
public:
    void Start() override;
    void Update() override;



    void SaveData();
    void SaveDataFromChest(int chestIndex, int chestContent);

    int levelIndex;

    int skillPoints;
    int unlockGunBlueprint;
    int upgradeBlueprintAmount;

    // casettes
    bool casette1Picked;
    bool casette2Picked;
    bool casette3Picked;

    // map indicator
    API_GameObject playerIndicatorGO;
    PlayerIndicator* playerIndicator;

    //BluePrints
    API_GameObject hud_blueprintsGO;
    HUD_BluePrints* hud_blueprints;

};

