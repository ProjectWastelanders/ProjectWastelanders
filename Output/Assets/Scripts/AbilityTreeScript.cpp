#include "AbilityTreeScript.h"
#include "InteractiveEnviroment/OpenMenuInterruptor.h"
#include "Player/PlayerMove.h"
#include "AbilityTreeUpgrades.h"

HELLO_ENGINE_API_C AbilityTreeScript* CreateAbilityTreeScript(ScriptToInspectorInterface* script)
{
    AbilityTreeScript* classInstance = new AbilityTreeScript();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIInput("Main Panel", &classInstance->mainPanel);
    script->AddDragBoxUIButton("Movement", &classInstance->AbilityList1);
    script->AddDragBoxUIButton("Armory", &classInstance->AbilityList2);
    script->AddDragBoxUIButton("Health", &classInstance->AbilityList3);
    script->AddDragBoxUIButton("Special", &classInstance->AbilityList4);

    //panels
    script->AddDragBoxGameObject("PanelMovement", &classInstance->PanelAbility[0]);
    script->AddDragBoxGameObject("PanelAbility2", &classInstance->PanelAbility[1]);
    script->AddDragBoxGameObject("PanelAbility3", &classInstance->PanelAbility[2]);
    script->AddDragBoxGameObject("PanelAbility4", &classInstance->PanelAbility[3]);

    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    return classInstance;
}

void AbilityTreeScript::Start()
{
    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (playerMove == nullptr) Console::Log("PlayerMove missing in AbilityTreeScript Script.");

    for (int i = 0; i < 4; ++i)
    {
        PanelAbility[i].SetActive(false);
        upgradeScripts[i] = (AbilityTreeUpgrades*)PanelAbility[i].GetScript("AbilityTreeUpgrades");
    }
}

void AbilityTreeScript::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && mainPanel.IsEnabled())
    {
        Audio::Event("click");
        if (playerMove) playerMove->openingChest = false;
        mainPanel.GetGameObject().SetActive(false);
        for (int i = 0; i < 4; ++i)
        {
            PanelAbility[i].SetActive(false);
        }
        return;
    }

    if (!mainPanel.GetGameObject().IsActive()) return;

    if (AbilityList1.OnHovered())
    {
        if (ability1)
        {
            Audio::Event("hover");
            ability1 = false;
            ability2 = true;
        }
        PanelAbility[0].SetActive(true);
        PanelAbility[1].SetActive(false);
        PanelAbility[2].SetActive(false);
        PanelAbility[3].SetActive(false);
    }
    else if (AbilityList2.OnHovered())
    {
        if (ability2)
        {
            Audio::Event("hover");
            ability2 = false;
            ability3 = true;
            ability1 = true;
        }
        PanelAbility[0].SetActive(false);
        PanelAbility[1].SetActive(true);
        PanelAbility[2].SetActive(false);
        PanelAbility[3].SetActive(false);
    }
    else if (AbilityList3.OnHovered())
    {
        if (ability3)
        {
            Audio::Event("hover");
            ability3 = false;
            ability2 = true;
            ability4 = true;
        }
        PanelAbility[0].SetActive(false);
        PanelAbility[1].SetActive(false);
        PanelAbility[2].SetActive(true);
        PanelAbility[3].SetActive(false);
    }
    else if (AbilityList4.OnHovered())
    {
        if (ability4)
        {
            Audio::Event("hover");
            ability4 = false;
            ability3 = true;
        }
        PanelAbility[0].SetActive(false);
        PanelAbility[1].SetActive(false);
        PanelAbility[2].SetActive(false);
        PanelAbility[3].SetActive(true);
    }
}
