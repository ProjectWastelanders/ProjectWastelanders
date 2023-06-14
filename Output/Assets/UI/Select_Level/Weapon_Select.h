#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class OpenMenuInterruptor;
class PlayerMove;

class Weapon_Select : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_UIButton weapons[4];

    API_UIButton proceedButton;

    API_UIImage infoWeapon;
    API_UIImage weaponImage;

    API_UIInput weaponSelectPanel;
    API_UIInput proceedPanel;

    API_String scene;

    uint info1;
    uint info2;
    uint info3;
    uint info4;

    uint weaponImage1;
    uint weaponImage2;
    uint weaponImage3;
    uint weaponImage4;

    bool isPress;
    bool inOpen;
    bool onProceed;
    bool firstTime;
    bool firstTimeLevel;
    int indexLevles;

    API_GameObject interruptorGO;

    API_GameObject playerGO;
    PlayerMove* playerMove;

    void ResetQuickSaveValues(int levelIndex);
};

