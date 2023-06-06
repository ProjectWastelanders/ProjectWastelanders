#include "PlayerStorage.h"
#include "../UI Test folder/PlayerIndicator.h"
#include "../InteractiveEnviroment/EnemyManager.h"
#include "../InteractiveEnviroment/BoxManager.h"
HELLO_ENGINE_API_C PlayerStorage* CreatePlayerStorage(ScriptToInspectorInterface* script)
{
    PlayerStorage* classInstance = new PlayerStorage();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Level Index", &classInstance->levelIndex);
    script->AddDragBoxGameObject("Map Indicator", &classInstance->playerIndicatorGO);
    script->AddDragBoxGameObject("Hud BluePrint Indicator GO", &classInstance->hud_blueprintsGO);
    script->AddDragBoxGameObject("Enemy Manager GO", &classInstance->enemyManagerGO);
    script->AddDragBoxGameObject("Box Manager GO", &classInstance->boxManagerGO);

    return classInstance;
}

void PlayerStorage::Start()
{
    skillPoints = API_QuickSave::GetInt("skillPointsAmount");
    unlockGunBlueprint = -1; // -1 --> no gun unlocked
    upgradeBlueprintAmount = API_QuickSave::GetInt("upgradeBlueprintAmount");

    switch (levelIndex)
    {
    case 0: // hub
        // no casettes in hub
        break;
    case 1: // level 1
        casette1Picked = API_QuickSave::GetBool("level1_casette1");
        casette2Picked = API_QuickSave::GetBool("level1_casette2");
        casette3Picked = API_QuickSave::GetBool("level1_casette3");
        break;
    case 2: // level 2
        casette1Picked = API_QuickSave::GetBool("level2_casette1");
        casette2Picked = API_QuickSave::GetBool("level2_casette2");
        casette3Picked = API_QuickSave::GetBool("level2_casette3");
        break;
    case 3: // level 3
        casette1Picked = API_QuickSave::GetBool("level3_casette1");
        casette2Picked = API_QuickSave::GetBool("level3_casette2");
        casette3Picked = API_QuickSave::GetBool("level3_casette3");
        break;
    case 4: // level 4
        casette1Picked = API_QuickSave::GetBool("level4_casette1");
        casette2Picked = API_QuickSave::GetBool("level4_casette2");
        casette3Picked = API_QuickSave::GetBool("level4_casette3");
        break;
    case 5: // level 5
        casette1Picked = API_QuickSave::GetBool("level5_casette1");
        casette2Picked = API_QuickSave::GetBool("level5_casette2");
        casette3Picked = API_QuickSave::GetBool("level5_casette3");
        break;
    default:
        Console::Log("Wrong level index", API::Console::MessageType::ERR);
        break;
    }

    if (levelIndex != 0)
    {
        gameObject.GetTransform().SetPosition(API_QuickSave::GetFloat("PlayerPosX"), API_QuickSave::GetFloat("PlayerPosY"), API_QuickSave::GetFloat("PlayerPosZ"));

        playerIndicator = (PlayerIndicator*)playerIndicatorGO.GetScript("PlayerIndicator");
        if (playerIndicator == nullptr) Console::Log("PlayerIndicator missing in PlayerStorage.");
    }

    hud_blueprints = (HUD_BluePrints*)hud_blueprintsGO.GetScript("HUD_BluePrints");
    if (hud_blueprints == nullptr) Console::Log("HUD_BluePrints missing in PlayerStorage.");
    
    hud_Alert_Prints = (Blue_Print_Screen_Alert*)hud_blueprintsGO.GetScript("Blue_Print_Screen_Alert");
    if (hud_blueprints == nullptr) Console::Log("Blue_Print_Screen_Alert missing in PlayerStorage.");

    enemyManager = (EnemyManager*)enemyManagerGO.GetScript("EnemyManager");
    if (enemyManager == nullptr) Console::Log("EnemyManager missing in PlayerStorage.");
    else enemyManager->LoadEnemiesState(levelIndex);

    boxManager = (BoxManager*)boxManagerGO.GetScript("BoxManager");
    if (boxManager == nullptr) Console::Log("BoxManager missing in PlayerStorage.");
    else boxManager->LoadBoxesState(levelIndex);
}

void PlayerStorage::Update()
{

}

void PlayerStorage::SaveData()
{

    // Player Position
    API_QuickSave::SetFloat("PlayerPosX", gameObject.GetTransform().GetGlobalPosition().x);
    API_QuickSave::SetFloat("PlayerPosY", gameObject.GetTransform().GetGlobalPosition().y);
    API_QuickSave::SetFloat("PlayerPosZ", gameObject.GetTransform().GetGlobalPosition().z);

    // Player Indicator Positiony
    if (playerIndicator) playerIndicator->SavePosition();

    // skill points
    API_QuickSave::SetInt("skillPointsAmount", skillPoints);

    // save unlocked gun
    switch (unlockGunBlueprint)
    {
    case 1:
        API_QuickSave::SetBool("semiautomaticGunBlueprint", true);
        break;
    case 2:
        API_QuickSave::SetBool("automaticGunBlueprint", true);
        break;
    case 3:
        API_QuickSave::SetBool("burstGunBlueprint", true);
        break;
    case 4:
        API_QuickSave::SetBool("shotgunGunBlueprint", true);
        break;
    case 5:
        API_QuickSave::SetBool("handgunGunBlueprint", true);
        break;
    default:
        break;
    }

    // save upgradeBlueprint amount
    API_QuickSave::SetInt("upgradeBlueprintAmount", upgradeBlueprintAmount);

    // save casettes
    switch (levelIndex)
    {
    case 0:
        break;
    case 1:
        API_QuickSave::SetBool("level1_casette1", casette1Picked);
        API_QuickSave::SetBool("level1_casette2", casette2Picked);
        API_QuickSave::SetBool("level1_casette3", casette3Picked);
        break;
    case 2:
        API_QuickSave::SetBool("level2_casette1", casette1Picked);
        API_QuickSave::SetBool("level2_casette2", casette2Picked);
        API_QuickSave::SetBool("level2_casette3", casette3Picked);
        break;
    case 3:
        API_QuickSave::SetBool("level3_casette1", casette1Picked);
        API_QuickSave::SetBool("level3_casette2", casette2Picked);
        API_QuickSave::SetBool("level3_casette3", casette3Picked);
        break;
    case 4:
        API_QuickSave::SetBool("level4_casette1", casette1Picked);
        API_QuickSave::SetBool("level4_casette2", casette2Picked);
        API_QuickSave::SetBool("level4_casette3", casette3Picked);
        break;
    case 5:
        API_QuickSave::SetBool("level5_casette1", casette1Picked);
        API_QuickSave::SetBool("level5_casette2", casette2Picked);
        API_QuickSave::SetBool("level5_casette3", casette3Picked);
        break;
    default:
        Console::Log("Wrong level index", API::Console::MessageType::ERR);
        break;
    }

    //enemies
    if (enemyManager) enemyManager->SaveEnemiesState(levelIndex);

    //boxes
    if (boxManager) boxManager->SaveBoxesState(levelIndex);
}

void PlayerStorage::SaveDataFromChest(int chestIndex, int chestContent)
{
    if (levelIndex == 0) return;

    std::string saveActiveLabel = "level" + std::to_string(levelIndex) + "_chest" + std::to_string(chestIndex + 1);
    std::string saveContentLabel = saveActiveLabel + "_content";

    API_QuickSave::SetBool(saveActiveLabel, true); // active == true
    
    if (chestContent <= 0) API_QuickSave::SetInt(saveContentLabel, 2); // chest have upgradeBlueprint
    else if (chestContent <= 5) API_QuickSave::SetInt(saveContentLabel, 1); // chest have gunBlueprint
    else API_QuickSave::SetInt(saveContentLabel, 0); // chest have specialGun
}