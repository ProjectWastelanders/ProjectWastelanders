#include "Thanos_HP_UI.h"
HELLO_ENGINE_API_C Thanos_HP_UI* CreateThanos_HP_UI(ScriptToInspectorInterface* script)
{
	Thanos_HP_UI* classInstance = new Thanos_HP_UI();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxGameObject("Boss Stats GO", &classInstance->thanosStatsGO);

    script->AddDragBoxGameObject("HUD BOSS BAR MINI", &classInstance->thanos_HUD);
    script->AddDragBoxUIImage("HP BOSS UI IMAGE", &classInstance->thanos_hp_bar);

    script->AddDragBoxTextureResource("HP BOSS TEXTURE", &classInstance->hp_thanos_texture);

	return classInstance;
}

void Thanos_HP_UI::Start()
{
    thanosStats = (ThanosLoop*)thanosStatsGO.GetScript("ThanosLoop");
    if (thanosStats == nullptr) Console::Log("Missing ThanosStats on Boss_Bar_HUD Script.");
    
    thanosMovment = (ThanosMovement*)thanosStatsGO.GetScript("ThanosMovement");
    if (thanosMovment == nullptr) Console::Log("Missing ThanosStats on Boss_Bar_HUD Script.");

    thanos_hp_bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(hp_thanos_texture);
}
void Thanos_HP_UI::Update()
{
    //Console::Log("Pol Rois" + std::to_string(bossStats->dist));
    if (thanosStats != nullptr)
    {
        if (thanosMovment->letsFight == true && thanosStats->hp > 0)
        {
           thanos_HUD.SetActive(true);
           Thanos_Boss_Bar(thanosStats->hp / 1500.0f);
        }
        else
        {
            thanos_HUD.SetActive(false);
        }


    }
}

void Thanos_HP_UI::Thanos_Boss_Bar(float HP_Boss_Value)
{
    thanos_hp_bar.FillImage(HP_Boss_Value);
    
}
