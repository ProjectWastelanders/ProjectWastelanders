#include "ChestManager.h"
#include "Chest.h"
#include "../Player/PlayerStorage.h"
HELLO_ENGINE_API_C ChestManager* CreateChestManager(ScriptToInspectorInterface* script)
{
    ChestManager* classInstance = new ChestManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Chest 1", &classInstance->chest1);
    script->AddDragBoxGameObject("Chest 2", &classInstance->chest2);
    script->AddDragBoxGameObject("Chest 3", &classInstance->chest3);
    script->AddDragBoxGameObject("Chest 4", &classInstance->chest4);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    return classInstance;
}

void ChestManager::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in ChestManager.");

    specialGunAsigned = false;
    if (playerStorage && playerStorage->levelIndex == 3)
    {
        gunBlueprintAsigned = true;
        remainingUpgradeBlueprints = 3;
    }
    else
    {
        gunBlueprintAsigned = false;
        remainingUpgradeBlueprints = 2;
    }

    check = false;
}

void ChestManager::Update()
{
    if (check || !playerStorage) return; // only do update 1 frame

    std::string saveActiveLabel = "level" + std::to_string(playerStorage->levelIndex) + "_chest";

    // skip???
    Chest* chestScript1 = (Chest*)chest1.GetScript("Chest");
    if (chestScript1)
    {
        bool chestActive = API_QuickSave::GetBool(saveActiveLabel + "1");
        Console::Log("chest 1: " + std::to_string(chestActive));
        if (chestActive) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "1" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript1->OpenChestOnStart();
        }
        skipChest1 = chestActive;
    }

    Chest* chestScript2 = (Chest*)chest2.GetScript("Chest");
    if (chestScript2)
    {
        bool chestActive = API_QuickSave::GetBool(saveActiveLabel + "2");
        Console::Log("chest 2: " + std::to_string(chestActive));
        if (chestActive) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "2" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript2->OpenChestOnStart();
        }
        skipChest2 = chestActive;
    }

    Chest* chestScript3 = (Chest*)chest3.GetScript("Chest");
    if (chestScript3)
    {
        bool chestActive = API_QuickSave::GetBool(saveActiveLabel + "3");
        Console::Log("chest 3: " + std::to_string(chestActive));
        if (chestActive) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "3" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript3->OpenChestOnStart();
        }
        skipChest3 = chestActive;
    }

    Chest* chestScript4 = (Chest*)chest4.GetScript("Chest");
    if (chestScript4)
    {
        bool chestActive = API_QuickSave::GetBool(saveActiveLabel + "4");
        Console::Log("chest 4: " + std::to_string(chestActive));
        if (chestActive) // if chest is not active on save
        {
            int loot = API_QuickSave::GetInt(saveActiveLabel + "4" + "_content");

            switch (loot)
            {
            case 0:
                specialGunAsigned = true;
                break;
            case 1:
                gunBlueprintAsigned = true;
                break;
            case 2:
                remainingUpgradeBlueprints--;
                break;
            default:
                Console::Log("Chest save is wrong.", API::Console::MessageType::ERR);
                break;
            }

            chestScript4->OpenChestOnStart();
        }
        skipChest4 = chestActive;
    }

    // set loot
    if (chestScript1 && !skipChest1)
    {
        if (chestScript1->tutorialSpecialWeapon) chestScript1->itemIndex = GetSpecialGun();
        else if (chestScript1->tutorialWeaponBlueprint) chestScript1->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();

            if (loot == 0)
            {
                chestScript1->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript1->itemIndex = GetGunBlueprint();
            }
            else if (loot == 2) // upgrade blueprint
            {
                chestScript1->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
        chestScript1->chestIndex = 0;
    }

    if (chestScript2 && !skipChest2)
    {
        if (chestScript2->tutorialSpecialWeapon) chestScript2->itemIndex = GetSpecialGun();
        else if (chestScript2->tutorialWeaponBlueprint) chestScript2->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();

            if (loot == 0)
            {
                chestScript2->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript2->itemIndex = GetGunBlueprint();
            }
            else if (loot == 2) // upgrade blueprint
            {
                chestScript2->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
        chestScript2->chestIndex = 1;
    }

    if (chestScript3 && !skipChest3)
    {
        if (chestScript3->tutorialSpecialWeapon) chestScript3->itemIndex = GetSpecialGun();
        else if (chestScript3->tutorialWeaponBlueprint) chestScript3->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();

            if (loot == 0)
            {
                chestScript3->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript3->itemIndex = GetGunBlueprint();
            }
            else if (loot == 2) // upgrade blueprint
            {
                chestScript3->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
        chestScript3->chestIndex = 2;
    }

    if (chestScript4 && !skipChest4)
    {
        if (chestScript4->tutorialSpecialWeapon) chestScript4->itemIndex = GetSpecialGun();
        else if (chestScript4->tutorialWeaponBlueprint) chestScript4->itemIndex = GetGunBlueprint();
        else
        {
            int loot = GetRandomLoot();

            if (loot == 0)
            {
                chestScript4->itemIndex = GetSpecialGun();
            }
            else if (loot == 1)
            {
                chestScript4->itemIndex = GetGunBlueprint();
            }
            else if (loot == 2) // upgrade blueprint
            {
                chestScript4->itemIndex = 0;
                remainingUpgradeBlueprints--;
            }
        }
        chestScript4->chestIndex = 3;
    }

    check = true;
}

int ChestManager::GetRandomLoot()
{
    if (specialGunAsigned && gunBlueprintAsigned && remainingUpgradeBlueprints == 0) return -1;

    int random = rand() % 4;

    if (random == 0)
    {
        if (!specialGunAsigned) return 0;
        else return GetRandomLoot();
    }
    else if (random == 1 && playerStorage && playerStorage->levelIndex != 3) // level 3 dont have gun blueprint
    {
        if (!gunBlueprintAsigned) return 1;
        else return GetRandomLoot();
    }
    else if (random > 1 || (random == 1 && playerStorage && playerStorage->levelIndex == 3))
    {
        if (remainingUpgradeBlueprints > 0) return 2;
        else return GetRandomLoot();
    }
}

int ChestManager::GetGunBlueprint()
{
    gunBlueprintAsigned = true;
    int random;
    bool gun1 = API_QuickSave::GetBool("semiautomaticGunBlueprint");
    bool gun2 = API_QuickSave::GetBool("automaticGunBlueprint");
    bool gun3 = API_QuickSave::GetBool("burstGunBlueprint");
    bool gun4 = API_QuickSave::GetBool("shotgunGunBlueprint");
    if (gun1 && gun2 && gun3 && gun4)
    {
        Console::Log("ChestManager funtion: GetGunBlueprint. That should never happend. Why can't have more than 4 normal guns...", API::Console::MessageType::ERR);
        return -1;
    }

    do
    {
        random = (rand() % 4) + 1;
        if ((random == 1 && gun1) || (random == 2 && gun2) || (random == 3 && gun3) || (random == 4 && gun4))
        {
            random = -1;
        }
        else
        {
            return random;
        }
    } while (random == -1);

    return random;
}

int ChestManager::GetSpecialGun()
{
    specialGunAsigned = true;
    int random = (rand() % 2) + 5;
    return random;
}