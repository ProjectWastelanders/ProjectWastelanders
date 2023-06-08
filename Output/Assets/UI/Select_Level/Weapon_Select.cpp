#include "Weapon_Select.h"
#include "../../Assets/Scripts/InteractiveEnviroment/OpenMenuInterruptor.h"
#include "../../Assets/Scripts/Player/PlayerMove.h"
#include "../../Scripts/UI Test folder/HUB/HUB_UIManager.h"

HELLO_ENGINE_API_C Weapon_Select* CreateWeapon_Select(ScriptToInspectorInterface* script)
{
    Weapon_Select* classInstance = new Weapon_Select();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIInput("Weapon Selected Panel", &classInstance->weaponSelectPanel);
    script->AddDragBoxUIInput("Proceed Panel", &classInstance->proceedPanel);

    script->AddDragBoxUIButton("Weapon 1", &classInstance->weapons[0]);
    script->AddDragBoxUIButton("Weapon 2", &classInstance->weapons[1]);
    script->AddDragBoxUIButton("Weapon 3", &classInstance->weapons[2]);
    script->AddDragBoxUIButton("Weapon 4", &classInstance->weapons[3]);

    script->AddDragBoxUIImage("Weapon Info", &classInstance->infoWeapon);

    script->AddDragBoxTextureResource("Info Weapon 1", &classInstance->info1);
    script->AddDragBoxTextureResource("Info Weapon 2", &classInstance->info2);
    script->AddDragBoxTextureResource("Info Weapon 3", &classInstance->info3);
    script->AddDragBoxTextureResource("Info Weapon 4", &classInstance->info4);

    script->AddDragBoxUIImage("Weapon Image", &classInstance->weaponImage);

    script->AddDragBoxTextureResource("Weapon Image 1", &classInstance->weaponImage1);
    script->AddDragBoxTextureResource("Weapon Image 2", &classInstance->weaponImage2);
    script->AddDragBoxTextureResource("Weapon Image 3", &classInstance->weaponImage3);
    script->AddDragBoxTextureResource("Weapon Image 4", &classInstance->weaponImage4);

    script->AddDragBoxUIButton("Proceed Button", &classInstance->proceedButton);

    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    return classInstance;
}

void Weapon_Select::Start()
{
    indexLevles = 3;
    inOpen = true;
    onProceed = false;
    firstTime = true;
    firstTimeLevel = true;

    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (playerMove == nullptr) Console::Log("PlayerMove missing in ArmoryWeaponSelect Script with gunIndex 0.");
}
void Weapon_Select::Update()
{
    if (inOpen)
    {
        if (API_QuickSave::GetBool("semiautomaticUnlocked"))
            weapons[0].SetBlocked(true);
        else
            weapons[0].SetBlocked(false);

        if (API_QuickSave::GetBool("automaticUnlocked"))
            weapons[1].SetBlocked(true);
        else
            weapons[1].SetBlocked(false);

        if (API_QuickSave::GetBool("burstUnlocked"))
            weapons[2].SetBlocked(true);
        else
            weapons[2].SetBlocked(false);

        if (API_QuickSave::GetBool("shotgunUnlocked"))
            weapons[3].SetBlocked(true);
        else
            weapons[3].SetBlocked(false);

        inOpen = false;
    }

    if ((Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > 10000 && isPress) && indexLevles >= 1 && weaponSelectPanel.GetGameObject().IsActive())
    {
        isPress = false;
        indexLevles--;
    }
    else if ((Input::GetGamePadButton(GamePadButton::BUTTON_UP) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < -10000 && isPress) && indexLevles <= 2 && weaponSelectPanel.GetGameObject().IsActive())
    {
        isPress = false;
        indexLevles++;
    }
    else if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > -10000 && Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < 10000)
    {
        isPress = true;
    }

    if (weaponSelectPanel.GetGameObject().IsActive())
    {
        for (int i = 0; i < 4; ++i)
        {
            if (weapons[i].OnPress())
            {
                weaponSelectPanel.GetGameObject().SetActive(false);
                proceedPanel.GetGameObject().SetActive(true);
                onProceed = true;
                API_QuickSave::SetInt("equipedNormalGun", indexLevles + 1);
            }
        }
    }

    switch (indexLevles)
    {
    case 3:
        infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info1);
        weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage1);
        break;
    case 2:
        infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info2);
        weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage2);
        break;
    case 1:
        infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info3);
        weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage3);
        break;
    case 0:
        infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info4);
        weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage4);
        break;
    }

    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
    {
        if (onProceed)
        {
            //Input::HandleGamePadButton(GamePadButton::BUTTON_B);
            onProceed = false;
            weaponSelectPanel.GetGameObject().SetActive(true);
            proceedPanel.GetGameObject().SetActive(false);
            firstTimeLevel = true;
        }
        else
        {
            HUB_UIManager::ClosePanel();
            /* weaponSelectPanel.GetGameObject().SetActive(false);*/
            gameObject.SetActive(false); // Crimen contra la humanidad, me disculpo. (Desactiva al padre porque si no se queda activado)
            //firstTime = true;
        }
    }

    if (proceedPanel.GetGameObject().IsActive() && Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_UP)
    {
        if (!firstTimeLevel)
        {
            int selectedLevel = API_QuickSave::GetInt("currentSelectedLevel");

            ResetQuickSaveValues(selectedLevel + 1);

            switch (selectedLevel)
            {
            case 0:
                API_QuickSave::SetFloat("PlayerPosX", 110.5f);
                API_QuickSave::SetFloat("PlayerPosY", 0.0f);
                API_QuickSave::SetFloat("PlayerPosZ", -29.2f);
                API_QuickSave::SetFloat("PlayerIndicatorPosX", -0.7f);
                API_QuickSave::SetFloat("PlayerIndicatorPosY", -0.39f);
                API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                scene = std::string("Level1.HScene");
                Scene::LoadScene(scene.c_str());
                break;
            case 1:
                if (API_QuickSave::GetBool("IsInMiddleOfLevel") == true)
                {
                    API_QuickSave::SetFloat("PlayerIndicatorPosX", -0.7f);
                    API_QuickSave::SetFloat("PlayerIndicatorPosY", -0.39f);
                    
                }
                else
                {
                    API_QuickSave::SetFloat("PlayerPosX", 147.6f);
                    API_QuickSave::SetFloat("PlayerPosY", 2.115f);
                    API_QuickSave::SetFloat("PlayerPosZ", 14.54f);
                    API_QuickSave::SetFloat("PlayerIndicatorPosX", 0);
                    API_QuickSave::SetFloat("PlayerIndicatorPosY", 0);
                    API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                }
                scene = std::string("Level2.HScene");
                Scene::LoadScene(scene.c_str());
                break;
            case 2:
                API_QuickSave::SetFloat("PlayerPosX", 90.866f);
                API_QuickSave::SetFloat("PlayerPosY", -4.099f);
                API_QuickSave::SetFloat("PlayerPosZ", -0.718f);
                API_QuickSave::SetFloat("PlayerIndicatorPosX", 0);
                API_QuickSave::SetFloat("PlayerIndicatorPosY", 0);
                API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                scene = std::string("Level3.HScene");
                Scene::LoadScene(scene.c_str());
                break;
            case 3:
                API_QuickSave::SetFloat("PlayerPosX", -52.75f);
                API_QuickSave::SetFloat("PlayerPosY", 10.68f);
                API_QuickSave::SetFloat("PlayerPosZ", 49.14f);
                API_QuickSave::SetFloat("PlayerIndicatorPosX", 0);
                API_QuickSave::SetFloat("PlayerIndicatorPosY", 0);
                API_QuickSave::SetBool("IsInMiddleOfLevel", false);
                scene = std::string("Level4.HScene");
                Scene::LoadScene(scene.c_str());
                break;
            }
        }

        HUB_UIManager::ClosePanel();

        firstTimeLevel = false;
    }
}

void Weapon_Select::ResetQuickSaveValues(int levelIndex)
{
    // guns
    API_QuickSave::SetInt("equipedSpecialGun", -1);
    API_QuickSave::SetInt("normalAmmo", -1);
    API_QuickSave::SetInt("specialAmmo", -1);

    // enemies
    std::string name = "lvl" + std::to_string(levelIndex) + "_enemy";
    for (size_t i = 0; i < 59; i++)
    {
        std::string variable = name + std::to_string(i);
        API_QuickSave::SetBool(variable.c_str(), true);
    }

    // boxes
    name = "lvl" + std::to_string(levelIndex) + "box";
    for (size_t i = 0; i < 30; i++)
    {
        std::string variable = name + std::to_string(i);
        API_QuickSave::SetBool(variable.c_str(), true);
    }
}