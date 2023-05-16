#include "PlayerGunManager.h"
#include "../UI Test folder/SwapWeapon.h"
HELLO_ENGINE_API_C PlayerGunManager* CreatePlayerGunManager(ScriptToInspectorInterface* script)
{
    PlayerGunManager* classInstance = new PlayerGunManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Stats GO", &classInstance->player);
    script->AddDragInt("Base Gun", &classInstance->gunOnHandIndex1);
    script->AddDragInt("Normal Gun", &classInstance->gunOnHandIndex2);
    script->AddDragInt("Special Gun", &classInstance->gunOnHandIndex3);
    script->AddDragBoxGameObject("Duals", &classInstance->duals);
    script->AddDragBoxGameObject("Semiautomatic", &classInstance->semiauto);
    script->AddDragBoxGameObject("Automatic", &classInstance->automatic);
    script->AddDragBoxGameObject("Burst", &classInstance->burst);
    script->AddDragBoxGameObject("Shotgun", &classInstance->shotgun);
    script->AddDragBoxGameObject("Flamethrower", &classInstance->flamethrower);
    script->AddDragBoxGameObject("Ricochet", &classInstance->ricochet);
    script->AddDragFloat("Swap Delay", &classInstance->maxSwapDelay);
    script->AddDragFloat("Fast Swap Delay", &classInstance->maxFastSwapDelay);
    script->AddDragBoxGameObject("Swap Weapon GO (HUD)", &classInstance->swapWeaponGO);
    return classInstance;
}

void PlayerGunManager::Start()
{
    playerStats = (PlayerStats*)player.GetScript("PlayerStats");
    if (playerStats == nullptr) Console::Log("Missing PlayerStats on PlayerGunManager Script.");
    playerMove = (PlayerMove*)player.GetScript("PlayerMove");
    if (playerMove == nullptr) Console::Log("Missing PlayerMove on PlayerGunManager Script.");

    swapWeapon = (SwapWeapon*)swapWeaponGO.GetScript("SwapWeapon");
    if (swapWeapon == nullptr) Console::Log("Missing SwapWeapon on PlayerGunManager Script.");

    // add guns to the array in order
    guns.push_back(duals);
    guns.push_back(semiauto);
    guns.push_back(automatic);
    guns.push_back(burst);
    guns.push_back(shotgun);
    guns.push_back(flamethrower);
    guns.push_back(ricochet);

    // get start guns
    //GetGun(1, 0);
    //int equipedNormalGun = API_QuickSave::GetInt("equipedNormalGun");
    //if (equipedNormalGun < -1 || equipedNormalGun > 4) equipedNormalGun = -1;
    //GetGun(2, equipedNormalGun);
    //GetGun(3, -1);
    GetGun(1, gunOnHandIndex1);
    GetGun(2, gunOnHandIndex2);
    GetGun(3, gunOnHandIndex3);
    playerStats->laserAmmo = 99999;
    playerStats->specialAmmo = 99999;
    //switch (equipedNormalGun)
    //{
    //case 1: // semiautomatic
    //    playerStats->maxLaserAmmo = 150;
    //    playerStats->laserAmmo = 150;
    //    if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::SEMI);
    //    break;
    //case 2: // automatic
    //    playerStats->maxLaserAmmo = 350;
    //    playerStats->laserAmmo = 350;
    //    if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::AUTO);
    //    break;
    //case 3: // burst
    //    playerStats->maxLaserAmmo = 100;
    //    playerStats->laserAmmo = 100;
    //    if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::BURST);
    //    break;
    //case 4: // shotgun
    //    playerStats->maxLaserAmmo = 70;
    //    playerStats->laserAmmo = 70;
    //    if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::SHOTGUN);
    //    break;
    //default:
    //    break;
    //}
    
    UnequipGun(0); // start with base gun selected
}

void PlayerGunManager::Update()
{
    float dt;
    if (playerStats && playerStats->slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (playerMove && playerMove->openingChest) return; // return if opening chest
    if (playerStats && playerStats->hittedTime > 0.0f) return; // return if hitted

    // Keyboard
    if (Input::GetKey(KeyCode::KEY_1) == KeyState::KEY_DOWN) UnequipGun(gunOnHandIndex1);
    else if (Input::GetKey(KeyCode::KEY_2) == KeyState::KEY_DOWN) UnequipGun(gunOnHandIndex2);
    else if (Input::GetKey(KeyCode::KEY_3) == KeyState::KEY_DOWN) UnequipGun(gunOnHandIndex3);

    // gamepad
    if (Input::GetGamePadButton(GamePadButton::BUTTON_LEFT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferRB > 0)
        {
            UnequipGun(gunOnHandIndex3); // special weapon
            bufferRB = 0.0f;
        }
        else bufferLB = 0.1f;
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_RIGHT_SHOULDER) == KeyState::KEY_DOWN)
    {
        if (bufferLB > 0.0f)
        {
            UnequipGun(gunOnHandIndex3); // special weapon
            bufferLB = 0.0f;
        }
        else bufferRB = 0.1f;
    }
    if (bufferLB > 0.0f)
    {
        bufferLB -= Time::GetRealTimeDeltaTime();
        if (bufferLB <= 0.0f)
        {
            UnequipGun(gunOnHandIndex1); // base weapon
            bufferLB = 0.0f;
        }
    }
    if (bufferRB > 0.0f)
    {
        bufferRB -= Time::GetRealTimeDeltaTime();
        if (bufferRB <= 0)
        {
            UnequipGun(gunOnHandIndex2); // normal weapon
            bufferRB = 0.0f;
        }
    }

    // swap gun
    if (swapDelay > 0.0f)
    {
        swapDelay -= dt;
        if (swapDelay <= 0.0f)
        {
            swapDelay = 0.0f;
            EquipGun(swapToIndex);
        }
        return;
    }

    if (equipedGun == nullptr || (playerMove && playerMove->isDashing)) return; // cant shoot if is dashing

    if (Input::GetGamePadAxis(GamePadAxis::AXIS_TRIGGERRIGHT) > 5000 || Input::GetMouseButton(MouseButton::LEFT) == KeyState::KEY_REPEAT)
    {
        if (playerStats && playerStats->GetAmmonByType(equipedGun->ammoType) > 0)
        {
            equipedGun->Shoot();
            if (playerMove) playerMove->PlayShootAnim(equipedIndex);
        }
        else
        {
            // no ammo sound?
        }
    }
}

void PlayerGunManager::GetGun(int slot, int gunIndex)
{
    switch (slot)
    {
    case 1:
        gunOnHandIndex1 = gunIndex;
        break;
    case 2:
        gunOnHandIndex2 = gunIndex;
        break;
    case 3:
        gunOnHandIndex3 = gunIndex;
        break;
    default:
        Console::Log("Invalid slot, slot should be 1, 2 or 3.");
        break;
    }

    UnequipGun(gunIndex);
}

void PlayerGunManager::EquipGun(int index)
{
    PlayerGunType* gunType = (PlayerGunType*)guns[index].GetScript("PlayerGunType");
    if (gunType == nullptr) return;

    switch (gunType->gunType)
    {
    case 0: // duals
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerDuals");
        break;
    case 1: // semiautomatic
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerSemiAuto");
        if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::SEMI);
        break;
    case 2: // automatic
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerAutomatic");
        if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::AUTO);
        break;
    case 3: // burst
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerBurst");
        if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::BURST);
        break;
    case 4: // shotgun
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerShotgun");
        if (swapWeapon) swapWeapon->SwapWeapon2(normalWeapon_Type::SHOTGUN);
        break;
    case 5: // flamethrower
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerFlamethrower");
        if (swapWeapon) swapWeapon->SwapWeapon3(specialWeapon_Type::FLAMETHROWER);
        break;
    case 6: // ricochet
        equipedGun = (PlayerGun*)guns[index].GetScript("PlayerRicochet");
        if (swapWeapon) swapWeapon->SwapWeapon3(specialWeapon_Type::RICOCHET);
        break;
    default:
        equipedGun = nullptr;
        break;
    }
    if (equipedGun != nullptr) equipedGun->EnableGuns(true);

    if (playerMove) playerMove->StopSwapGunAnim();
}

void PlayerGunManager::UnequipGun(int index)
{
    if (index == -1) return;

    if (playerMove)
    {
        if (equipedIndex == 0) playerMove->PlaySwapGunAnim(0);
        else playerMove->PlaySwapGunAnim(1);
    }

    if (equipedGun != nullptr) equipedGun->EnableGuns(false);
    equipedIndex = index;

    if (playerStats && playerStats->armoryTreeLvl > 0) swapDelay = maxFastSwapDelay + 0.001f;
    else swapDelay = maxSwapDelay + 0.001f;
    swapToIndex = index;
}