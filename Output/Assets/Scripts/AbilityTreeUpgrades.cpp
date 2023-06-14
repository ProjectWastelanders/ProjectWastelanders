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
    script->AddDragBoxUIInput("Current Bought Panel", &classInstance->currentBoughtPanel);
    script->AddDragInt("Tree Index", &classInstance->treeIndex);
    script->AddDragBoxGameObject("Player", &classInstance->playerStorageGO);

    script->AddDragBoxGameObject("Blocked ability Image", &classInstance->blockedAbility);

    script->AddDragBoxUIButton("Bought upgrade 1", &classInstance->boughtButtons[0]);
    script->AddDragBoxUIButton("Bought upgrade 2", &classInstance->boughtButtons[1]);
    script->AddDragBoxUIButton("Bought upgrade 3", &classInstance->boughtButtons[2]);
    script->AddDragBoxUIButton("Bought upgrade 4", &classInstance->boughtButtons[3]);
    script->AddDragBoxUIButton("Bought upgrade 5", &classInstance->boughtButtons[4]);
    return classInstance;
}

void AbilityTreeUpgrades::Init()
{
    // Get hold images from the 3 upgrade buttons
    Upgrate1.GetGameObject().GetChildren(&upgradeHoldImages[0], 1);
    Upgrate2.GetGameObject().GetChildren(&upgradeHoldImages[1], 1);
    Upgrate3.GetGameObject().GetChildren(&upgradeHoldImages[2], 1);
    Upgrate4.GetGameObject().GetChildren(&upgradeHoldImages[3], 1);
    Upgrate5.GetGameObject().GetChildren(&upgradeHoldImages[4], 1);
}

void AbilityTreeUpgrades::Start()
{
    skillLevel = API_QuickSave::GetInt("tree" + std::to_string(treeIndex) + "_level");

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("Player Storage missing in AbilitytreeUpdates.");
    currentPanel.SetEnable(false);
    currentBoughtPanel.SetEnable(false);

    if (skillLevel != 0) Upgrate1.SetBlocked(true);
    if (skillLevel != 1) Upgrate2.SetBlocked(true);
    if (skillLevel != 2) Upgrate3.SetBlocked(true);
    if (skillLevel != 3 && upgradeNum > 3) Upgrate4.SetBlocked(true);
    if (skillLevel != 4 && upgradeNum > 4) Upgrate5.SetBlocked(true);  

    for (int i = 0; i < 5; ++i)
    {
        boughtButtons[i].GetGameObject().SetActive(false);
    }

    nonBoughtButtons[0] = Upgrate1;
    nonBoughtButtons[1] = Upgrate2;
    nonBoughtButtons[2] = Upgrate3;
    nonBoughtButtons[3] = Upgrate4;
    nonBoughtButtons[4] = Upgrate5;

}

void AbilityTreeUpgrades::Update()
{
    if (!isOn)
    {
        for (int i = 0; i < 5; ++i)
        {
            upgradeHoldImages[i].SetActive(false);
        }
        return;
    }
    currentBoughtPanel.SetEnable(true);

    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && currentPanel.IsEnabled())
    {
        Audio::Event("click");
        Input::HandleGamePadButton(GamePadButton::BUTTON_B);
        mainPanel.SetEnable(true);
        currentPanel.SetEnable(false);
        currentBoughtPanel.SetEnable(false);
        isOn = false;
        blockedAbility.SetActive(false);
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

    upgradeHoldImages[0].SetActive((Upgrate1.OnHovered() || Upgrate1.OnPress() || Upgrate1.OnHold()) && skillLevel == 0);
    upgradeHoldImages[1].SetActive((Upgrate2.OnHovered() || Upgrate2.OnPress() || Upgrate2.OnHold()) && skillLevel == 1);
    upgradeHoldImages[2].SetActive((Upgrate3.OnHovered() || Upgrate3.OnPress() || Upgrate3.OnHold()) && skillLevel == 2);
    upgradeHoldImages[3].SetActive((Upgrate4.OnHovered() || Upgrate4.OnPress() || Upgrate4.OnHold()) && skillLevel == 3);
    upgradeHoldImages[4].SetActive((Upgrate5.OnHovered() || Upgrate5.OnPress() || Upgrate5.OnHold()) && skillLevel == 4);


    ShowBlockedImage(); // Logic that shows the Blocked image when hovering a blocked skill.

    for (int i = 0; i < 5; ++i)
    {
        if (skillLevel >= i + 1)
        {
            nonBoughtButtons[i].GetGameObject().SetActive(false);
            boughtButtons[i].GetGameObject().SetActive(true);
        }
        else
        {
            nonBoughtButtons[i].GetGameObject().SetActive(true);
            boughtButtons[i].GetGameObject().SetActive(false);
        }
    }

    if (Upgrate1.OnPress() && manteinTime == 0.0f)
    {
        if (playerStorage->skillPoints >= skillPoints1 && skillLevel == 0)
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
        if (playerStorage->skillPoints >= skillPoints2 && skillLevel == 1)
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
        if (playerStorage->skillPoints >= skillPoints3 && skillLevel == 2)
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
        if (playerStorage->skillPoints >= skillPoints4 && skillLevel == 3)
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
        if (playerStorage->skillPoints >= skillPoints5 && skillLevel == 4)
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

void AbilityTreeUpgrades::OnEnable()
{
    blockedAbility.SetActive(false);
}

void AbilityTreeUpgrades::OnDisable()
{
    currentBoughtPanel.SetEnable(false);
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

void AbilityTreeUpgrades::ShowBlockedImage()
{
    if (Upgrate1.OnHovered() || Upgrate1.OnPress())
        blockedAbility.SetActive(false);

    if (Upgrate2.OnHovered() || Upgrate2.OnPress())
    {
        if (skillLevel < 1)
            blockedAbility.SetActive(true);
        else
            blockedAbility.SetActive(false);
    }
    if (Upgrate3.OnHovered() || Upgrate3.OnPress())
    {
        if (skillLevel < 2)
            blockedAbility.SetActive(true);
        else
            blockedAbility.SetActive(false);
    }
    if (Upgrate4.OnHovered() || Upgrate4.OnPress())
    {
        if (skillLevel < 3)
            blockedAbility.SetActive(true);
        else
            blockedAbility.SetActive(false);
    }
    if (Upgrate5.OnHovered() || Upgrate5.OnPress())
    {
        if (skillLevel < 4)
            blockedAbility.SetActive(true);
        else
            blockedAbility.SetActive(false);
    }
}
