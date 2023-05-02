#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "SwapWeapon.h"
#include "Macro.h"
#include "API/API_UIText.h"
#include "API/API.h"

#include "../Player/PlayerStats.h"
#include "../Shooting/PlayerGunManager.h"

class UI_Municion : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject text_munition_InfinitWeapon_On;
    API_GameObject text_munition_InfinitWeapon_Off;

    API_UIImage image_more_munition;
    API_UIText text_munition_Normal_On;
    API_UIText text_munition_Normal_Off;
    API_UIText text_munition_Special_On;
    API_UIText text_munition_Special_Off;
    API_UIText text_more_munition_Normal;

    API_GameObject playerStatsGO;
    PlayerStats* playerStats;
    PlayerGunManager* playerGunManager;

    float opacity_Munition = 0;
    bool opacity_Active = false;
    bool opacity_Reset = true;
};

