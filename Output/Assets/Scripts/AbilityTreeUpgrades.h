#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "Player/PlayerStorage.h"

class AbilityTreeUpgrades : HelloBehavior
{
public:
    void Init() override;
    void Start() override;
    void Update() override;

    void OnEnable() override;

    void OnDisable() override;

    void UpgradeSkill();
    void ShowBlockedImage();

    int upgradeCost;

    API_UIButton Upgrate1;
    int skillPoints1;
    API_UIButton Upgrate2;
    int skillPoints2;
    API_UIButton Upgrate3;
    int skillPoints3;
    API_UIButton Upgrate4;
    int skillPoints4;
    API_UIButton Upgrate5;
    int skillPoints5;
    int upgradeNum;

    API_GameObject upgradeHoldImages[5];

    API_UIInput mainPanel;
    API_UIInput currentPanel;
    API_UIInput currentBoughtPanel;

    API_GameObject blockedAbility;

    API_UIButton boughtButtons[5];
    API_UIButton nonBoughtButtons[5];

    int skillLevel;
    int treeIndex;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;

    float manteinTime = 0.0f;
    int upgradingLevel;

    bool isOn = false;

};

