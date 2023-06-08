#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../UI Test folder/HUD_BluePrints.h"
#include "../UI Test folder/Blue_Print_Screen_Alert.h"
#include "API/API.h"

class PlayerIndicator;
class EnemyManager;
class BoxManager;

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
    Blue_Print_Screen_Alert* hud_Alert_Prints;

    // enemies
    API_GameObject enemyManagerGO;
    EnemyManager* enemyManager;

    // boxes
    API_GameObject boxManagerGO;
    BoxManager* boxManager;
};

