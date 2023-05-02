#include "HpBar.h"
HELLO_ENGINE_API_C HpBar* CreateHpBar(ScriptToInspectorInterface* script)
{
    HpBar* classInstance = new HpBar();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

    script->AddDragBoxGameObject("Player Stats GO", &classInstance->playerStatsGO);
    script->AddDragBoxGameObject("HUD fons", &classInstance->fons_movment_GO);
    script->AddDragBoxUIImage("HP_BAR", &classInstance->hp_Bar);
    script->AddDragBoxUIImage("SHIELD_BAR", &classInstance->shield_Bar);

    script->AddDragBoxTextureResource("HP Texture", &classInstance->hp_texture);
    script->AddDragBoxTextureResource("HP Regen Texture", &classInstance->hp_regen_texture);
    return classInstance;
}

void HpBar::Start()
{

    playerStats = (PlayerStats*)playerStatsGO.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on HpBar Script.");

    fons_movment= (HUD_SHIELD_BROKEN*)fons_movment_GO.GetScript("HUD_SHIELD_BROKEN");
    if (fons_movment == nullptr) Console::Log("HUD_SHIELD_BROKEN PlayerStats on HpBar Script.");

    hp_Bar.FillImage(1);
    hp_Bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(hp_texture);
}

void HpBar::Update()
{

    if (playerStats)
    {
        float deathlineHp;
        if (playerStats->healthTreeLvl > 1) deathlineHp = playerStats->currentMaxHp * (playerStats->upgradedDeadlinePart / 100.0f);
        else deathlineHp = playerStats->currentMaxHp * (playerStats->deadlinePart / 100.0f);
        if (playerStats->currentHp < deathlineHp)
        {
            hp_regen = true;

        }
        else if (playerStats->currentHp >= deathlineHp)
        {
            hp_regen = false;

        }

        HpPlayerbar(playerStats->currentHp / playerStats->currentMaxHp, hp_regen);
        ShieldPlayerbar(playerStats->shield / 50);
    }


}

void HpBar::HpPlayerbar(float vida_Player_value, bool Regen_Bar_Active)
{

    if (Regen_Bar_Active == false)
    {
        hp_Bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(hp_texture);
    }
    else if (Regen_Bar_Active == true)
    {
        hp_Bar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(hp_regen_texture);
    }

    hp_Bar.FillImage(vida_Player_value);
}

void HpBar::ShieldPlayerbar(float Shield_Player_value)
{
    shield_Bar.FillImage(Shield_Player_value);
    
    if (shield_broked== true)
    {
        if (Shield_Player_value <= 0 && shield_broked == true)
        {
            fons_movment->breack_shield_Player_anim = true;
            shield_broked = false;
        }
    }
    else if (Shield_Player_value > 0)
    {
        shield_broked = true;
    }

}