#include "Thanos_HP_UI.h"
HELLO_ENGINE_API_C Thanos_HP_UI* CreateThanos_HP_UI(ScriptToInspectorInterface* script)
{
	Thanos_HP_UI* classInstance = new Thanos_HP_UI();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxGameObject("Boss Stats GO", &classInstance->thanosStatsGO);

    script->AddDragBoxGameObject("HUD BOSS BAR MINI", &classInstance->thanos_HUD);
    script->AddDragBoxUIImage("HP BOSS UI IMAGE", &classInstance->thanos_hp_bar);

	return classInstance;
}

void Thanos_HP_UI::Start()
{
    thanosStats = (ThanosLoop*)thanosStatsGO.GetScript("ThanosLoop");
    if (thanosStats == nullptr) Console::Log("Missing ThanosStats on Boss_Bar_HUD Script.");
    
    thanosMovment = (ThanosMovement*)thanosStatsGO.GetScript("ThanosMovement");
    if (thanosMovment == nullptr) Console::Log("Missing ThanosStats on Boss_Bar_HUD Script.");

}
void Thanos_HP_UI::Update()
{
    //Console::Log("Pol Rois" + std::to_string(bossStats->dist));
    if (thanosStats != nullptr)
    {
        if (thanosStats->phase < 3 && thanosStats->hp > 0)
        {
            if (thanosMovment->letsFight == true)
            {
                shield_Fight_On = true;
            }

           Thanos_Boss_Bar(thanosStats->hp / 3000.0f);
        }
        else
        {
            shield_Fight_On = true;
        }
    }
    thanos_HUD.SetActive(shield_Fight_On);
}

void Thanos_HP_UI::Thanos_Boss_Bar(float HP_Boss_Value)
{
    thanos_hp_bar.FillImage(HP_Boss_Value);
    
}
