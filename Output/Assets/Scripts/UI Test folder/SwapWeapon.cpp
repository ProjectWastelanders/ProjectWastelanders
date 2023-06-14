#include "SwapWeapon.h"
#include "../Shooting/PlayerGunManager.h"
HELLO_ENGINE_API_C SwapWeapon* CreateSwapWeapon(ScriptToInspectorInterface* script)
{
    SwapWeapon* classInstance = new SwapWeapon();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    //infinitWeapons 
    //on
    script->AddDragBoxTextureResource("HandgunsWeapons_On", &classInstance->infinitWeaponsOn[0]);
    //off                              
    script->AddDragBoxTextureResource("HandgunsWeapons_Off", &classInstance->infinitWeaponsOff[0]);


    //NormalWeapons 
    //on
    script->AddDragBoxTextureResource("Auto_On", &classInstance->normalWeaponsOn[1]);
    script->AddDragBoxTextureResource("Burst_On", &classInstance->normalWeaponsOn[2]);
    script->AddDragBoxTextureResource("Semi_On", &classInstance->normalWeaponsOn[3]);
    script->AddDragBoxTextureResource("Shotgun_On", &classInstance->normalWeaponsOn[4]);
    script->AddDragBoxTextureResource("Handgun_On", &classInstance->normalWeaponsOn[5]);
    //off
    script->AddDragBoxTextureResource("Auto_Off", &classInstance->normalWeaponsOff[1]);
    script->AddDragBoxTextureResource("Burst_Off", &classInstance->normalWeaponsOff[2]);
    script->AddDragBoxTextureResource("Semi_Off", &classInstance->normalWeaponsOff[3]);
    script->AddDragBoxTextureResource("Shotgun_Off", &classInstance->normalWeaponsOff[4]);
    script->AddDragBoxTextureResource("Handgun_Off", &classInstance->normalWeaponsOff[5]);


    //EspecialesWeapons 
    //on
    script->AddDragBoxTextureResource("FlameThrower_On 1", &classInstance->specialsWeaponsOn[1]);
    script->AddDragBoxTextureResource("Ricochet_On 2", &classInstance->specialsWeaponsOn[2]);
    //off                              
    script->AddDragBoxTextureResource("FlameThrower_Off 1", &classInstance->specialsWeaponsOff[1]);
    script->AddDragBoxTextureResource("Ricochet_Off 2", &classInstance->specialsWeaponsOff[2]);


    //weapon 1
    script->AddDragBoxMaterialComponent("Material Weapon 1 on", &classInstance->material_Infinite_Weapon_on);
    script->AddDragBoxMaterialComponent("Material Weapon 1 off", &classInstance->material_Infinite_Weapon_off);
    script->AddDragBoxGameObject("Game Bar Weapon 1", &classInstance->active_Bar_1);

    //weapon 2
    script->AddDragBoxMaterialComponent("Material Weapon 2 on", &classInstance->material_Normal_Weapon_on);
    script->AddDragBoxMaterialComponent("Material Weapon 2 off", &classInstance->material_Normal_Weapon_off);
    script->AddDragBoxGameObject("Game Bar Weapon 2", &classInstance->active_Bar_2);

    //weapon 3
    script->AddDragBoxMaterialComponent("Material Weapon 3 on", &classInstance->material_Special_Weapon_on);
    script->AddDragBoxMaterialComponent("Material Weapon 3 off", &classInstance->material_Special_Weapon_off);
    script->AddDragBoxGameObject("Game Bar Weapon 3", &classInstance->active_Bar_3);

    ////dash
    script->AddDragBoxTextureResource("Text_Dash_on", &classInstance->texture_Dash_on);
    script->AddDragBoxTextureResource("Text_Dash_off", &classInstance->texture_Dash_off);

    script->AddDragBoxMaterialComponent("Material_Dash", &classInstance->material_Dash);

    script->AddDragBoxGameObject("Player Move GO", &classInstance->playerGO);

    return classInstance;
}

void SwapWeapon::Start()
{
    //weapon 1
    material_Infinite_Weapon_on.ChangeAlbedoTexture(infinitWeaponsOn[0]);
    material_Infinite_Weapon_off.ChangeAlbedoTexture(infinitWeaponsOff[0]);
    material_Infinite_Weapon_off.GetGameObject().SetActive(false);
    active_Bar_1.SetActive(true);

    //weapon 2
    material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[0]);
    material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[0]);
    material_Normal_Weapon_on.GetGameObject().SetActive(false);
    material_Normal_Weapon_off.GetGameObject().SetActive(true);
    active_Bar_2.SetActive(false);
    //weapon 3
    material_Special_Weapon_on.ChangeAlbedoTexture(specialsWeaponsOn[0]);
    material_Special_Weapon_off.ChangeAlbedoTexture(specialsWeaponsOff[0]);
    material_Special_Weapon_on.GetGameObject().SetActive(false);
    material_Special_Weapon_off.GetGameObject().SetActive(true);
    active_Bar_3.SetActive(false);

    //Dash
    dashingReady = true;
    material_Dash.ChangeAlbedoTexture(texture_Dash_on);

    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (playerMove == nullptr) Console::Log("PlayerMove missing in SwapWeapon Script.");

    playerGunManager = (PlayerGunManager*)playerGO.GetScript("PlayerGunManager");
    if (playerGunManager == nullptr) Console::Log("PlayerGunManager missing in SwapWeapon Script.");
}
void SwapWeapon::Update()
{
    int equipedIndex = 0;

    if (playerGunManager)
    {
        switch (playerGunManager->equipedIndex)
        {
        case 0:
            equipedIndex = 0;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            equipedIndex = 1;
            break;
        case 5:
        case 6:
            equipedIndex = 2;
            break;
        default:
            break;
        }
    }

    if (equipedIndex == 0)
    {
        //weapons on
        material_Infinite_Weapon_on.GetGameObject().SetActive(true);
        material_Normal_Weapon_on.GetGameObject().SetActive(false);
        material_Special_Weapon_on.GetGameObject().SetActive(false);

        //weapons off
        material_Infinite_Weapon_off.GetGameObject().SetActive(false);
        material_Normal_Weapon_off.GetGameObject().SetActive(true);
        material_Special_Weapon_off.GetGameObject().SetActive(true);

        //bars
        active_Bar_1.SetActive(true);
        active_Bar_2.SetActive(false);
        active_Bar_3.SetActive(false);
    }
    else if (equipedIndex == 1)
    {
        //weapons on
        material_Infinite_Weapon_on.GetGameObject().SetActive(false);
        material_Normal_Weapon_on.GetGameObject().SetActive(true);
        material_Special_Weapon_on.GetGameObject().SetActive(false);

        //weapons off
        material_Infinite_Weapon_off.GetGameObject().SetActive(true);
        material_Normal_Weapon_off.GetGameObject().SetActive(false);
        material_Special_Weapon_off.GetGameObject().SetActive(true);

        //bars
        active_Bar_1.SetActive(false);
        active_Bar_2.SetActive(true);
        active_Bar_3.SetActive(false);
    }
    else
    {
        //weapons on
        material_Infinite_Weapon_on.GetGameObject().SetActive(false);
        material_Normal_Weapon_on.GetGameObject().SetActive(false);
        material_Special_Weapon_on.GetGameObject().SetActive(true);

        //weapons off
        material_Infinite_Weapon_off.GetGameObject().SetActive(true);
        material_Normal_Weapon_off.GetGameObject().SetActive(true);
        material_Special_Weapon_off.GetGameObject().SetActive(false);

        //bars
        active_Bar_1.SetActive(false);
        active_Bar_2.SetActive(false);
        active_Bar_3.SetActive(true);
    }

    active_None_Weapon();

    if (playerMove && playerMove->dashesAvailable > 0 && !dashingReady)
    {
        Dash();
    }
    else if (playerMove && playerMove->dashesAvailable == 0 && dashingReady)
    {
        Dash();
    }
}

void SwapWeapon::SwapWeapon1(infinitWeapon_Type infinitWeapon)
{
    switch (infinitWeapon)
    {
    case infinitWeapon_Type::HANDGUNS:
        material_Infinite_Weapon_on.ChangeAlbedoTexture(infinitWeaponsOn[0]);
        material_Infinite_Weapon_off.ChangeAlbedoTexture(infinitWeaponsOff[0]);
        break;
    default:
        break;
    }
}

void SwapWeapon::SwapWeapon2(normalWeapon_Type normalWeapon)
{
    switch (normalWeapon)
    {
    case normalWeapon_Type::NONE:
        material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[0]);
        material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[0]);
        normal_NONE = true;
        break;
    case normalWeapon_Type::AUTO:
        material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[1]);
        material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[1]);
        normal_NONE = false;
        break;
    case normalWeapon_Type::BURST:
        material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[2]);
        material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[2]);
        normal_NONE = false;
        break;
    case normalWeapon_Type::SEMI:
        material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[3]);
        material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[3]);
        normal_NONE = false;
        break;
    case normalWeapon_Type::SHOTGUN:
        material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[4]);
        material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[4]);
        normal_NONE = false;
        break;
    case normalWeapon_Type::REVOLVER:
        material_Normal_Weapon_on.ChangeAlbedoTexture(normalWeaponsOn[5]);
        material_Normal_Weapon_off.ChangeAlbedoTexture(normalWeaponsOff[5]);
        normal_NONE = false;
        break;
    default:
        break;
    }
}

void SwapWeapon::SwapWeapon3(specialWeapon_Type specialWeapon)
{
    switch (specialWeapon)
    {
    case specialWeapon_Type::NONE:
        material_Special_Weapon_on.ChangeAlbedoTexture(specialsWeaponsOn[0]);
        material_Special_Weapon_off.ChangeAlbedoTexture(specialsWeaponsOff[0]);
        special_NONE = true;
        break;
    case specialWeapon_Type::FLAMETHROWER:
        material_Special_Weapon_on.ChangeAlbedoTexture(specialsWeaponsOn[1]);
        material_Special_Weapon_off.ChangeAlbedoTexture(specialsWeaponsOff[1]);
        special_NONE = false;
        break;
    case specialWeapon_Type::RICOCHET:
        material_Special_Weapon_on.ChangeAlbedoTexture(specialsWeaponsOn[2]);
        material_Special_Weapon_off.ChangeAlbedoTexture(specialsWeaponsOff[2]);
        special_NONE = false;
        break;
    default:
        break;
    }
}

void SwapWeapon::active_None_Weapon()
{
    if (normal_NONE == true)
    {
        material_Normal_Weapon_on.GetGameObject().GetMeshRenderer().SetActive(false);
        material_Normal_Weapon_off.GetGameObject().GetMeshRenderer().SetActive(false);
    }
    else
    {
        material_Normal_Weapon_on.GetGameObject().GetMeshRenderer().SetActive(true);
        material_Normal_Weapon_off.GetGameObject().GetMeshRenderer().SetActive(true);
    }

    if (special_NONE == true)
    {
        material_Special_Weapon_on.GetGameObject().GetMeshRenderer().SetActive(false);
        material_Special_Weapon_off.GetGameObject().GetMeshRenderer().SetActive(false);
    }
    else
    {
        material_Special_Weapon_on.GetGameObject().GetMeshRenderer().SetActive(true);
        material_Special_Weapon_off.GetGameObject().GetMeshRenderer().SetActive(true);
    }
}

void SwapWeapon::Dash()
{
    dashingReady = !dashingReady;
    if (!dashingReady)
    {
        material_Dash.ChangeAlbedoTexture(texture_Dash_off);
    }
    else
    {
        material_Dash.ChangeAlbedoTexture(texture_Dash_on);
    }
}

