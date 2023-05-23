#include "AbilitySelect.h"
#include "AbilityTreeUpgrades.h"
HELLO_ENGINE_API_C AbilitySelect* CreateAbilitySelect(ScriptToInspectorInterface* script)
{
    AbilitySelect* classInstance = new AbilitySelect();

    script->AddDragBoxUIInput("Main Panel", &classInstance->mainPanel);
    script->AddDragBoxUIInput("Current Panel", &classInstance->abilityPanel);
    script->AddDragBoxUIButton("AbilityButton", &classInstance->abilitySelect);

    script->AddDragBoxGameObject("Description 1", &classInstance->description1);
    script->AddDragBoxGameObject("Description 2", &classInstance->description2);
    script->AddDragBoxGameObject("Description 3", &classInstance->description3);
    script->AddDragBoxGameObject("Description 4", &classInstance->description4);
    script->AddDragBoxGameObject("Description 5", &classInstance->description5);
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void AbilitySelect::Start()
{
    abilityPanel.SetEnable(false);
    abilityTreeScript = (AbilityTreeScript*)mainPanel.GetGameObject().GetScript("AbilityTreeScript");
}

void AbilitySelect::Update()
{
    if (abilitySelect.OnHovered() && mainPanel.IsEnabled())
    {
        description1.SetActive(false);
        description2.SetActive(false);
        description3.SetActive(false);
        description4.SetActive(false);
        description5.SetActive(false);
        waitFrame = false;
    }
    else if (abilitySelect.OnPress() && !waitFrame && !abilityTreeScript->tutorialActive)
    {
        Audio::Event("click");

        mainPanel.SetEnable(false);
        abilityPanel.SetEnable(true);

        AbilityTreeUpgrades* panelScript = (AbilityTreeUpgrades*)abilityPanel.GetGameObject().GetScript("AbilityTreeUpgrades");
        panelScript->isOn = true;
        waitFrame = true;
    }
    else if (!abilitySelect.OnHovered())
    {
        waitFrame = true;
    }
}