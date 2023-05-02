#include "ArmoryUpgratteButtons.h"
HELLO_ENGINE_API_C ArmoryUpgratteButtons* CreateArmoryUpgratteButtons(ScriptToInspectorInterface* script)
{
    ArmoryUpgratteButtons* classInstance = new ArmoryUpgratteButtons();

    script->AddDragBoxUIButton("Upgrate 1", &classInstance->Upgrate1);
    script->AddDragBoxUIButton("Upgrate 2", &classInstance->Upgrate2);
    script->AddDragBoxUIButton("Upgrate 3", &classInstance->Upgrate3);

    script->AddDragBoxUIInput("Panel Upgrate", &classInstance->PanelUpgrate);

    script->AddDragBoxGameObject("Player", &classInstance->Player);

    script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);

    return classInstance;
}

void ArmoryUpgratteButtons::Start()
{
    switch (gunIndex)
    {
    case 0:
        gunLevel = API_QuickSave::GetInt("duals_level");
        break;
    case 1:
        gunLevel = API_QuickSave::GetInt("semiautomatic_level");
        break;
    case 2:
        gunLevel = API_QuickSave::GetInt("automatic_level");
        break;
    case 3:
        gunLevel = API_QuickSave::GetInt("burst_level");
        break;
    case 4:
        gunLevel = API_QuickSave::GetInt("shotgun_level");
        break;
    case 5:
        gunLevel = API_QuickSave::GetInt("handgun_level");
        break;
    default:
        break;
    }

    _playerStorage = (PlayerStorage*)Player.GetScript("PlayerStorage");
    if (_playerStorage == nullptr) Console::Log("Player Storage missing in ArmaryUpgradeButtons.");

    if (gunLevel != 0) Upgrate1.SetBlocked(true);
    if (gunLevel != 1) Upgrate2.SetBlocked(true);
    if (gunLevel != 2) Upgrate3.SetBlocked(true);

    gameObject.SetActive(false);
}

void ArmoryUpgratteButtons::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && PanelUpgrate.IsEnabled())
    {
        Audio::Event("click");
        Input::HandleGamePadButton(GamePadButton::BUTTON_B);
        SelectWeaponList.SetEnable(true);
        PanelUpgrate.SetEnable(false);
    }

    if (!_playerStorage) return;

    if (manteinTime > 0.0f)
    {
        manteinTime -= Time::GetRealTimeDeltaTime();

        if (Upgrate1.OnRelease() || Upgrate2.OnRelease() || Upgrate3.OnRelease())
        {
            manteinTime = 0.0f;
        }
        else if (manteinTime <= 0.0f)
        {
            manteinTime = 0.0f;

            if (upgradingLevel == 1)
            {
                UpgradeGun();
                Upgrate1.SetBlocked(true);
                Upgrate2.SetBlocked(false);
            }
            else if (upgradingLevel == 2)
            {
                UpgradeGun();
                Upgrate2.SetBlocked(true);
                Upgrate3.SetBlocked(false);
            }
            else if (upgradingLevel == 3)
            {
                UpgradeGun();
                Upgrate3.SetBlocked(true);
            }
        }
    }

    if (Upgrate1.OnPress() && manteinTime == 0.0f)
    {
        if (_playerStorage->upgradeBlueprintAmount > 0)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 1;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate2.OnPress() && manteinTime == 0.0f)
    {
        if (_playerStorage->upgradeBlueprintAmount > 0)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 2;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate3.OnPress() && manteinTime == 0.0f)
    {
        if (_playerStorage->upgradeBlueprintAmount > 0)
        {
            Audio::Event("click");
            manteinTime = 1.0f;
            upgradingLevel = 3;
        }
        else
        {
            Audio::Event("error");
            // wrong sound
        }
    }

    if (Upgrate1.getIsBlocked() && Upgrate1.OnPress())
    {
        Audio::Event("error");
    }
    if (Upgrate2.getIsBlocked() && Upgrate2.OnPress())
    {
        Audio::Event("error");
    }
    if (Upgrate3.getIsBlocked() && Upgrate3.OnPress())
    {
        Audio::Event("error");
    }
    if (Upgrate1.OnHovered() && audioUp1)
    {
        audioUp1 = false;
        audioUp2 = true;
        Audio::Event("hover");
    }
    if (Upgrate2.OnHovered() && audioUp2)
    {
        audioUp1 = true;
        audioUp2 = false;
        audioUp3 = true;
        Audio::Event("hover");
    }
    if (Upgrate3.OnHovered() && audioUp3)
    {
        audioUp2 = true;
        audioUp3 = false;
        Audio::Event("hover");
    }
    /*if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_RIGHTY) >= 10000)
    {
        Console::Log("PEPE");
    }*/
}

void ArmoryUpgratteButtons::UpgradeGun()
{
    _playerStorage->upgradeBlueprintAmount--;
    _playerStorage->SaveData();

    Audio::Event("w_upgrade");

    gunLevel++;

    switch (gunIndex)
    {
    case 0:
        API_QuickSave::SetInt("duals_level", gunLevel);
        break;
    case 1:
        API_QuickSave::SetInt("semiautomatic_level", gunLevel);
        break;
    case 2:
        API_QuickSave::SetInt("automatic_level", gunLevel);
        break;
    case 3:
        API_QuickSave::SetInt("burst_level", gunLevel);
        break;
    case 4:
        API_QuickSave::SetInt("shotgun_level", gunLevel);
        break;
    case 5:
        API_QuickSave::SetInt("handgun_level", gunLevel);
        break;
    default:
        break;
    }
}