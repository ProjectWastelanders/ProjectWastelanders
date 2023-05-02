#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIImage.h"
#include "../UI Test folder/HUD_SHIELD_BROKEN.h"

#include "../Player/PlayerStats.h"



class HpBar : HelloBehavior
{
public:
    void Start() override;
    void Update() override;
    void HpPlayerbar(float vida_Player_value, bool Regen_Bar_Active);
    void ShieldPlayerbar(float Shield_Player_value);


    API_GameObject playerStatsGO;
    PlayerStats* playerStats;

    API_GameObject fons_movment_GO;
    HUD_SHIELD_BROKEN* fons_movment;

    uint hp_texture;
    uint hp_regen_texture;


    API_UIImage hp_Bar;
    API_UIImage shield_Bar;

    float hp_regen = false;

    bool shield_broked = false;

};
