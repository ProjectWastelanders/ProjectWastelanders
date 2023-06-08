#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "../Shooting/PlayerGunManager.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

class Chest : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionStay(API::API_RigidBody other) override;
    void OnCollisionEnter(API::API_RigidBody other) override;
    void OnCollisionExit(API::API_RigidBody other) override;

    void OpenChestOnStart();

    API_AnimationPlayer chestAnimatorPlayer;

    float openChestTime;
    float openChestTimeBar;
    float maxOpenChestTime = 1.0f;
    bool opening;
    bool opened = false;
    PlayerGunManager* playerGunManager;
    PlayerStats* playerStats;
    PlayerMove* playerMove;

    bool tutorialSpecialWeapon;
    bool tutorialWeaponBlueprint;
    bool bluprintTutorial;

    int chestIndex;
    int itemIndex;
    bool canGetGun = true;

    API_UIImage Tutorial_Img;

    API_Vector3 initalPos;
    API_Vector3 finalPos;
    API_Vector3 movingPos;
    
    bool activeTutorial = false;
    bool endTutorial = false;
    bool timerTutorial = false;
    bool hideChest = false;

    bool saveChest = true;

    float showTutorial = 0;

    API_UIImage guideButton;

    // 0 -> Upgrade Blueprint
    // 1 -> Unlock SemiAutomatic
    // 2 -> Unlock Automatic
    // 3 -> Unlock Burst
    // 4 -> Unlock Shotgun
    // 5 -> Unlock Handgun
    // 6 -> Get Flamethrower
    // 7 -> Get Ricochet
};