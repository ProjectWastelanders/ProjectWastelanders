#include "ArmoryWeaponSelect.h"
#include "InteractiveEnviroment/OpenMenuInterruptor.h"
#include "Player/PlayerMove.h"
HELLO_ENGINE_API_C ArmoryWeaponSelect* CreateArmoryWeaponSelect(ScriptToInspectorInterface* script)
{
    ArmoryWeaponSelect* classInstance = new ArmoryWeaponSelect();

    script->AddDragBoxUIButton("Weapon", &classInstance->CurrentWeapon);

    script->AddDragBoxGameObject("Next Weapon", &classInstance->NextWeapon);

    script->AddDragBoxGameObject("Prev Weapon", &classInstance->PrevtWeapon);

    script->AddDragBoxUIInput("Panel Upgrate", &classInstance->CurrentPanelUpgrate);
    script->AddDragBoxUIInput("Panel Unlock", &classInstance->CurrentPanelUnlock);

    script->AddDragBoxGameObject("Next Panel Upgrate", &classInstance->NextPanelUpgrate);
    script->AddDragBoxGameObject("Next Panel Unlock", &classInstance->NextPanelUnlock);

    script->AddDragBoxGameObject("Prev Panel Upgrate", &classInstance->PrevPanelUpgrate);
    script->AddDragBoxGameObject("Prev Panel Unlock", &classInstance->PrevPanelUnlock);

    script->AddDragBoxTextureResource("Material Weapon", &classInstance->CurrentTextureWeapon);
    script->AddDragBoxTextureResource("Material Weapon Locked", &classInstance->CurrentTextureWeaponLocked);

    script->AddDragBoxMaterialComponent("SelectedWeapon", &classInstance->SelectedWeapon);

    script->AddDragBoxUIInput("List Weapons", &classInstance->SelectWeaponList);

    script->AddDragInt("Gun Index", &classInstance->gunIndex);
    script->AddDragBoxGameObject("Open Menu Interruptor", &classInstance->interruptorGO);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);

    return classInstance;
}

void ArmoryWeaponSelect::Start()
{
    nextW = (ArmoryWeaponSelect*)NextWeapon.GetScript("ArmoryWeaponSelect");
    PrevW = (ArmoryWeaponSelect*)PrevtWeapon.GetScript("ArmoryWeaponSelect");
    CurrentPanelUnlock.SetEnable(false);

    interruptor = (OpenMenuInterruptor*)interruptorGO.GetScript("OpenMenuInterruptor");
    if (interruptor == nullptr && gunIndex == 0) Console::Log("OpenMenuInterruptor missing in ArmoryWeaponSelect Script with gunIndex 0.");

    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (playerMove == nullptr && gunIndex == 0) Console::Log("PlayerMove missing in ArmoryWeaponSelect Script with gunIndex 0.");

    FindUnlock();
}
void ArmoryWeaponSelect::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && SelectWeaponList.IsEnabled())
    {
        Audio::Event("click");

        findUnlock = true;
        if (!interruptor) return;
        Input::HandleGamePadButton(GamePadButton::BUTTON_B);
        // IT'S CORRECT DON'T REMOVE NOTHING
        interruptor->menuPanel.SetActive(true); // can set false if is not true
        interruptor->menuPanel.SetActive(false);
        if (playerMove) playerMove->openingChest = false;
        interruptor->open = false;  
        return;
    }
   
    if ((CurrentWeapon.OnHovered() || CurrentWeapon.OnPress()) && SelectWeaponList.IsEnabled() && findUnlock)
    {
        FindUnlock();
        if (isUnlocked)
        {
            SelectedWeapon.ChangeAlbedoTexture(CurrentTextureWeapon);
            CurrentPanelUnlock.GetGameObject().SetActive(false);
            CurrentPanelUpgrate.GetGameObject().SetActive(true);
        }
        else
        {
            SelectedWeapon.ChangeAlbedoTexture(CurrentTextureWeaponLocked);
            CurrentPanelUnlock.GetGameObject().SetActive(true);
        }

        if (nextW != nullptr)
        {
            NextPanelUpgrate.SetActive(false);
            NextPanelUnlock.SetActive(false);
        }
        if (PrevW != nullptr)
        {
            PrevPanelUpgrate.SetActive(false);
            PrevPanelUnlock.SetActive(false);
        }

        findUnlock = false;
    }
    else if (CurrentWeapon.OnPress() && !findUnlock)
    {
        Audio::Event("click");
        SelectWeaponList.SetEnable(false);
        if (isUnlocked)
        {
            CurrentPanelUpgrate.SetEnable(true);
        }
        else
        {
            CurrentPanelUnlock.SetEnable(true);
        }
        findUnlock = true;
    }
    else if (!CurrentWeapon.OnHovered())
    {
        findUnlock = true;
    }
}

void ArmoryWeaponSelect::FindUnlock()
{
    Audio::Event("hover");

    if (gunIndex == 0) isUnlocked = true;
    else
    {
        switch (gunIndex)
        {
        case 1:
            isUnlocked = API_QuickSave::GetBool("semiautomaticUnlocked");
            break;
        case 2:
            isUnlocked = API_QuickSave::GetBool("automaticUnlocked");
            break;
        case 3:
            isUnlocked = API_QuickSave::GetBool("burstUnlocked");
            break;
        case 4:
            isUnlocked = API_QuickSave::GetBool("shotgunUnlocked");
            break;
        case 5:
            isUnlocked = API_QuickSave::GetBool("handgunUnlocked");
            break;
        default:
            break;
        }
    }
}