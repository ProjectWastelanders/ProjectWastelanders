#include "AbilityTreeUpgrades.h"
HELLO_ENGINE_API_C AbilityTreeUpgrades* CreateAbilityTreeUpgrades(ScriptToInspectorInterface* script)
{
    AbilityTreeUpgrades* classInstance = new AbilityTreeUpgrades();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
    script->AddDragInt("Skillpoints needed LvL 1", &classInstance->skillPoints1);
    script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
    script->AddDragInt("Skillpoints needed LvL 2", &classInstance->skillPoints2);
    script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);
    script->AddDragInt("Skillpoints needed LvL 3", &classInstance->skillPoints3);
    script->AddDragBoxUIButton("Upgrate 4", &classInstance->Upgrate4);
    script->AddDragInt("Skillpoints needed LvL 4", &classInstance->skillPoints4);
    script->AddDragBoxUIButton("Upgrate 5", &classInstance->Upgrate5);
    script->AddDragInt("Skillpoints needed LvL 5", &classInstance->skillPoints5);
    script->AddDragInt("Upgrade Number", &classInstance->upgradeNum);
    script->AddDragBoxUIInput("Main Panel", &classInstance->mainPanel);
    script->AddDragBoxUIInput("Current Panel", &classInstance->currentPanel);
    script->AddDragInt("Tree Index", &classInstance->treeIndex);
    script->AddDragBoxGameObject("Player", &classInstance->playerStorageGO);
    return classInstance;
}

void AbilityTreeUpgrades::Start()
{
    skillLevel = API_QuickSave::GetInt("tree" + std::to_string(treeIndex) + "_level");

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("Player Storage missing in AbilitytreeUpdates.");
    currentPanel.SetEnable(false);

    if (skillLevel != 0) Upgrate1.SetBlocked(true);
    if (skillLevel != 1) Upgrate2.SetBlocked(true);
    if (skillLevel != 2) Upgrate3.SetBlocked(true);
    if (skillLevel != 3 && upgradeNum > 3) Upgrate4.SetBlocked(true);
    if (skillLevel != 4 && upgradeNum > 4) Upgrate5.SetBlocked(true);  
}

void AbilityTreeUpgrades::Update()
{
    if (!isOn)
        return;
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && currentPanel.IsEnabled())
    {
        Audio::Event("click");
        Input::HandleGamePadButton(GamePadButton::BUTTON_B);
        mainPanel.SetEnable(true);
        currentPanel.SetEnable(false);
        isOn = false;
        return;
    }

    if (!playerStorage) return;

    if (manteinTime > 0.0f)
    {
        manteinTime -= Time::GetRealTimeDeltaTime();

        if (Upgrate1.OnRelease() || Upgrate2.OnRelease() || Upgrate3.OnRelease() || Upgrate4.OnRelease() || Upgrate5.OnRelease())
        {
            manteinTime = 0.0f;
        }
        else if (manteinTime <= 0.0f)
        {
            manteinTime = 0.0f;

            if (upgradingLevel == 1)
            {
                UpgradeSkill();
                Upgrate1.SetBlocked(true);
                Upgrate2.SetBlocked(false);

            }
            else if (upgradingLevel == 2)
            {
                UpgradeSkill();
                Upgrate2.SetBlocked(true);
                Upgrate3.SetBlocked(false);
            }
            else if (upgradingLevel == 3)
            {
                UpgradeSkill();
                Upgrate3.SetBlocked(true);
                if (upgradeNum > 3) Upgrate4.SetBlocked(false);

            }
            else if (upgradingLevel == 4)
            {
                UpgradeSkill();
                Upgrate4.SetBlocked(true);
                if (upgradeNum > 4) Upgrate5.SetBlocked(false);
            }
            else if (upgradingLevel == 5)
            {
                UpgradeSkill();
                Upgrate5.SetBlocked(true);
            }
        }
    }

    if (Upgrate1.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints >= skillPoints1 && skillLevel < 1)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 1;
            upgradeCost = skillPoints1;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate2.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints >= skillPoints2 && skillLevel < 2)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 2;
            upgradeCost = skillPoints2;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate3.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints >= skillPoints3 && skillLevel < 3)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 3;
            upgradeCost = skillPoints3;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate4.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints >= skillPoints4 && skillLevel < 4)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 4;
            upgradeCost = skillPoints4;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate5.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints >= skillPoints5 && skillLevel < 5)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 5;
            upgradeCost = skillPoints5;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }
}

void AbilityTreeUpgrades::UpgradeSkill()
{
    Audio::Event("ability_get");
    playerStorage->skillPoints -= upgradeCost;
    playerStorage->SaveData();

    skillLevel++;
    API_QuickSave::SetInt("tree" + std::to_string(treeIndex) + "_level", skillLevel);
    Audio::Event("ability_get");
}