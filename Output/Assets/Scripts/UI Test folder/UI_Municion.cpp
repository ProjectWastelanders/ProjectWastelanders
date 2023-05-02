#include "UI_Municion.h"
HELLO_ENGINE_API_C UI_Municion* CreateUI_Municion(ScriptToInspectorInterface* script)
{
    UI_Municion* classInstance = new UI_Municion();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    script->AddDragBoxGameObject("Infinit munition on", &classInstance->text_munition_InfinitWeapon_On);
    script->AddDragBoxGameObject("Infinit munition off", &classInstance->text_munition_InfinitWeapon_Off);
    script->AddDragBoxUIImage("More munition Image", &classInstance->image_more_munition);

    script->AddDragBoxUIText("Normal munition on", &classInstance->text_munition_Normal_On);
    script->AddDragBoxUIText("Normal munition off", &classInstance->text_munition_Normal_Off);
    script->AddDragBoxUIText("Special munition on", &classInstance->text_munition_Special_On);
    script->AddDragBoxUIText("Special munition off", &classInstance->text_munition_Special_Off);
   // script->AddDragBoxUIText("More munition Text", &classInstance->text_more_munition_Normal);

    //
    return classInstance;
}

void UI_Municion::Start()
{
    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (!playerStats) Console::Log("PlayerStats Missing in UI_Municion.");

    playerGunManager = (PlayerGunManager*)playerStatsGO.GetScript("PlayerGunManager");
    if (!playerGunManager) Console::Log("PlayerGunManager Missing in UI_Municion.");

    text_munition_Normal_On.GetGameObject().GetMeshRenderer().SetActive(false);
    text_munition_Normal_Off.GetGameObject().GetMeshRenderer().SetActive(false);
    text_munition_Special_On.GetGameObject().GetMeshRenderer().SetActive(false);
    text_munition_Special_Off.GetGameObject().GetMeshRenderer().SetActive(false);
    
    //text_more_munition_Normal.GetGameObject().GetMeshRenderer().SetActive(false);

    text_munition_InfinitWeapon_On.SetActive(true);
    text_munition_InfinitWeapon_Off.SetActive(false);
    image_more_munition.SetOpacity(0);
    text_munition_Normal_On.GetGameObject().SetActive(false);
    text_munition_Normal_Off.GetGameObject().SetActive(false);
    text_munition_Special_On.GetGameObject().SetActive(false);
    text_munition_Special_Off.GetGameObject().SetActive(false);
    
    //text_more_munition_Normal.GetGameObject().SetActive(false);
}

void UI_Municion::Update()
{

    if (opacity_Active == true)
    {
        //si es 0 ho resetea a 1
        if (opacity_Munition <= 0 && opacity_Reset == true)
        {
            opacity_Munition = 1;
            image_more_munition.SetOpacity(opacity_Munition);
            opacity_Reset = false;
        }
        //redueix fins a 0
        else if(opacity_Reset == false)
        {
            image_more_munition.SetOpacity(opacity_Munition = opacity_Munition - 0.01);
            if (opacity_Munition <= 0)
            {
                opacity_Active = false;
                Console::Log("SI?");
                
            }
        }
    }
    else
    {
        opacity_Reset = true;
    }

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
        case 5:
            equipedIndex = 1;
            break;
        case 6:
        case 7:
            equipedIndex = 2;
            break;
        default:
            break;
        }
    }



    if (!playerStats) return;

    if (equipedIndex == 0)
    {
        text_munition_InfinitWeapon_On.SetActive(true);
        text_munition_InfinitWeapon_Off.SetActive(false);
        text_munition_Normal_On.SetText(std::to_string(playerStats->laserAmmo).c_str());
        text_munition_Normal_Off.SetText(std::to_string(playerStats->laserAmmo).c_str());
        text_munition_Normal_On.GetGameObject().SetActive(false);
        text_munition_Normal_Off.GetGameObject().SetActive(true);
        text_munition_Special_On.SetText(std::to_string(playerStats->specialAmmo).c_str());
        text_munition_Special_Off.SetText(std::to_string(playerStats->specialAmmo).c_str());
        text_munition_Special_On.GetGameObject().SetActive(false);
        text_munition_Special_Off.GetGameObject().SetActive(true);
    }
    else if (equipedIndex == 1)
    {
        text_munition_InfinitWeapon_On.SetActive(false);
        text_munition_InfinitWeapon_Off.SetActive(true);
        text_munition_Normal_On.SetText(std::to_string(playerStats->laserAmmo).c_str());
        text_munition_Normal_Off.SetText(std::to_string(playerStats->laserAmmo).c_str());
        text_munition_Normal_On.GetGameObject().SetActive(true);
        text_munition_Normal_Off.GetGameObject().SetActive(false);
        text_munition_Special_On.SetText(std::to_string(playerStats->specialAmmo).c_str());
        text_munition_Special_Off.SetText(std::to_string(playerStats->specialAmmo).c_str());
        text_munition_Special_On.GetGameObject().SetActive(false);
        text_munition_Special_Off.GetGameObject().SetActive(true);
    }
    else
    {
        text_munition_InfinitWeapon_On.SetActive(false);
        text_munition_InfinitWeapon_Off.SetActive(true);
        text_munition_Normal_On.SetText(std::to_string(playerStats->laserAmmo).c_str());
        text_munition_Normal_Off.SetText(std::to_string(playerStats->laserAmmo).c_str());
        text_munition_Normal_On.GetGameObject().SetActive(false);
        text_munition_Normal_Off.GetGameObject().SetActive(true);
        text_munition_Special_On.SetText(std::to_string(playerStats->specialAmmo).c_str());
        text_munition_Special_Off.SetText(std::to_string(playerStats->specialAmmo).c_str());
        text_munition_Special_On.GetGameObject().SetActive(true);
        text_munition_Special_Off.GetGameObject().SetActive(false);
    }
}