#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"
#include "API/API_UIText.h"
#include "ArmoryWeaponSelect.h"
#include "Player/PlayerStorage.h"

class ArmoryUpgratteButtons : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void UpgradeGun();

    API_UIButton Upgrate1;
    API_UIButton Upgrate2;
    API_UIButton Upgrate3;

    API_UIInput PanelUpgrate;

    API_UIInput SelectWeaponList;

    PlayerStorage* _playerStorage;
    API_GameObject Player;

    int gunIndex;
    int gunLevel;

    float manteinTime = 0.0f;
    int upgradingLevel;

    bool audioUp1 = true;
    bool audioUp2 = true;
    bool audioUp3 = true;
};

