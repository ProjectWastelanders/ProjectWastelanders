#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include <vector>

#include "API/API.h"

#include "PlayerGun.h"
#include "PlayerGunType.h"
#include "../Player/PlayerStats.h"
#include "../Player/PlayerMove.h"

class SwapWeapon;

class PlayerGunManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void GetGun(int slot, int gunIndex);
    void EquipGun(int index);
    void UnequipGun(int index);

    API_GameObject player;
    PlayerStats* playerStats;

    std::vector<API_GameObject> guns;
    int equipedIndex;
    PlayerGun* equipedGun = nullptr;
    int gunOnHandIndex1 = -1;
    int gunOnHandIndex2 = -1;
    int gunOnHandIndex3 = -1;
    int swapToIndex;
    float maxSwapDelay = 1.0f;
    float maxFastSwapDelay = 0.5f;
    float swapDelay;

    bool canShoot = true;
    float bufferRB;
    float bufferLB;

    // guns
    API_GameObject duals; // index 0
    API_GameObject semiauto; // index 1
    API_GameObject automatic; // index 2
    API_GameObject burst; // index 3
    API_GameObject shotgun; // index 4
    API_GameObject flamethrower; // index 5
    API_GameObject ricochet; // index 6

    // animations
    PlayerMove* playerMove;

    API_GameObject swapWeaponGO;
    SwapWeapon* swapWeapon;

    int saveNullWeapon;
};