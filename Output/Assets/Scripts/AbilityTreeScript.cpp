#include "AbilityTreeScript.h"
#include "InteractiveEnviroment/OpenMenuInterruptor.h"
#include "Player/PlayerMove.h"
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
    script->AddDragBoxGameObject("PanelMovement", &classInstance->PanelAbility1);
    script->AddDragBoxGameObject("PanelAbility2", &classInstance->PanelAbility2);
    script->AddDragBoxGameObject("PanelAbility3", &classInstance->PanelAbility3);
    script->AddDragBoxGameObject("PanelAbility4", &classInstance->PanelAbility4);

    script->AddDragBoxGameObject("Open Menu Interruptor", &classInstance->interruptorGO);
    script->AddDragBoxGameObject("Player", &classInstance->playerGO);
    return classInstance;
}

void AbilityTreeScript::Start()
{
    interruptor = (OpenMenuInterruptor*)interruptorGO.GetScript("OpenMenuInterruptor");
    if (interruptor == nullptr) Console::Log("OpenMenuInterruptor missing in AbilityTreeScript Script.");

    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (playerMove == nullptr) Console::Log("PlayerMove missing in AbilityTreeScript Script.");

    PanelAbility1.SetActive(false);
    PanelAbility2.SetActive(false);
    PanelAbility3.SetActive(false);
    PanelAbility4.SetActive(false);
    gameObject.SetActive(false);
}

void AbilityTreeScript::Update()
{
    if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN && mainPanel.IsEnabled())
    {
        Audio::Event("click");
        if (!interruptor) return;
        // IT'S CORRECT DON'T REMOVE NOTHING
        interruptor->menuPanel.SetActive(true); // can set false if is not true
        interruptor->menuPanel.SetActive(false);
        if (playerMove) playerMove->openingChest = false;
        interruptor->open = false;
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
        PanelAbility1.SetActive(true);
        PanelAbility2.SetActive(false);
        PanelAbility3.SetActive(false);
        PanelAbility4.SetActive(false);
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
        PanelAbility1.SetActive(false);
        PanelAbility2.SetActive(true);
        PanelAbility3.SetActive(false);
        PanelAbility4.SetActive(false);
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
        PanelAbility1.SetActive(false);
        PanelAbility2.SetActive(false);
        PanelAbility3.SetActive(true);
        PanelAbility4.SetActive(false);
    }
    else if (AbilityList4.OnHovered())
    {
        if (ability4)
        {
            Audio::Event("hover");
            ability4 = false;
            ability3 = true;
        }
        PanelAbility1.SetActive(false);
        PanelAbility2.SetActive(false);
        PanelAbility3.SetActive(false);
        PanelAbility4.SetActive(true);
    }
}
