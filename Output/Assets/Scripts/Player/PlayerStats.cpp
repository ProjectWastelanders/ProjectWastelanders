#include "PlayerStats.h"
#include "PlayerMove.h"
#include "../Enemies/EnemyDrop.h"
#include "../UI Test folder/HpBar.h"
#include "../UsefulScripts/IndexContainer.h"
#include "../UI Test folder/HUD_Power_Up_Scrip.h"
#include "../UI Test folder/Power_Ups_Alert.h"
#include "../UI Test folder/UI_Municion.h"
#include "../CamMov.h"

HELLO_ENGINE_API_C PlayerStats* CreatePlayerStats(ScriptToInspectorInterface* script)
{
    PlayerStats* classInstance = new PlayerStats();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("Current HP", &classInstance->currentHp);
    script->AddDragFloat("Upgraded Max HP", &classInstance->upgradedMaxHp);
    script->AddDragFloat("Deadline Percentage", &classInstance->deadlinePart);
    script->AddDragFloat("Upgraded Deadline Percentage", &classInstance->upgradedDeadlinePart);
    script->AddDragFloat("Deadline Heal Amount", &classInstance->deadlineHeal);
    script->AddDragFloat("Upgraded Deadline Heal Amount", &classInstance->upgradedDeadlineHeal);
    script->AddDragFloat("Aid Kit Heal Amount", &classInstance->aidKitHeal);
    script->AddDragFloat("Upgraded Aid Kit Heal Amount", &classInstance->upgradedAidKitHeal);
    script->AddDragBoxShaderComponent("Material Component", &classInstance->material);
    script->AddDragBoxGameObject("Player GO", &classInstance->playerGO);
    script->AddDragBoxGameObject("Power Ups Managers (HUD)", &classInstance->hudPowerUpGO);
    script->AddDragBoxGameObject("Hud Munition GO", &classInstance->ammo_ScriptGO);
    script->AddDragBoxGameObject("Camera", &classInstance->cameraGameObject);
    //script->AddDragInt("movement tree lvl", &classInstance->movementTreeLvl); // use it only for playtesting
    //script->AddDragInt("armory tree lvl", &classInstance->armoryTreeLvl);
    //script->AddDragInt("health tree lvl", &classInstance->healthTreeLvl);
    //script->AddDragInt("special tree lvl", &classInstance->specialTreeLvl);
    return classInstance;
}

void PlayerStats::Start()
{
    movementTreeLvl = API_QuickSave::GetInt("tree0_level");
    armoryTreeLvl = API_QuickSave::GetInt("tree1_level");
    healthTreeLvl = API_QuickSave::GetInt("tree2_level");
    specialTreeLvl = API_QuickSave::GetInt("tree3_level");

    if (healthTreeLvl > 0) currentMaxHp = upgradedMaxHp;
    else currentMaxHp = maxHp;
    currentHp = currentMaxHp;
    healingFromDeathline = false;

    detected = false;

    if (healthTreeLvl > 4) secondLife = true;
    else secondLife = false;

    speedPowerUp = 0;
    fireratePowerUp = 0;
    shield = 0;
    slowTimePowerUp = 0;

    storage = (PlayerStorage*)playerGO.GetScript("PlayerStorage");
    if (!storage) Console::Log("Storage Missing in PlayerStats. Only needed in levels.");

    playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
    if (!playerMove) Console::Log("PlayerMove Missing in PlayerStats.");

    hudPowerUp = (HUD_Power_Up_Scrip*)hudPowerUpGO.GetScript("HUD_Power_Up_Scrip");
    if (!hudPowerUp) Console::Log("HUD_Power_Up_Scrip Missing in PlayerStats. Only needed in levels.");
    
    hudPowerUp_Alert = (Power_Ups_Alert*)hudPowerUpGO.GetScript("Power_Ups_Alert");
    if (!hudPowerUp_Alert) Console::Log("Power_Ups_Alert Missing in PlayerStats. Only needed in levels.");
    
    ammo_Script = (UI_Municion*)ammo_ScriptGO.GetScript("UI_Municion");
    if (!ammo_Script) Console::Log("UI_Municion Missing in PlayerStats. Only needed in levels.");

    cam = (CamMov*)cameraGameObject.GetScript("CamMov");
    if (!cam) Console::Log("CamMov Missing in PlayerStats. Only needed in levels.");
}

void PlayerStats::Update()
{
    float dt;
    if (slowTimePowerUp > 0.0f /*&& !paused*/) dt = Time::GetRealTimeDeltaTime();
    else dt = Time::GetDeltaTime();

    if (Input::GetKey(KeyCode::KEY_K) == KeyState::KEY_DOWN)
    {
        TakeDamage(50, 0);
    }
    if (Input::GetKey(KeyCode::KEY_J) == KeyState::KEY_DOWN)
    {
        GetAmmo(1, 100);
    }
    if (Input::GetKey(KeyCode::KEY_H) == KeyState::KEY_DOWN)
    {
        UseAmmo(1, 100);
    }

    // deadline healing
    float deathlineHp;
    if (healthTreeLvl > 1) deathlineHp = currentMaxHp * (upgradedDeadlinePart / 100.0f);
    else deathlineHp = currentMaxHp * (deadlinePart / 100.0f);
    if (currentHp < deathlineHp && PlayerAlive())
    {
        lastHitTime -= dt;
        if (lastHitTime <= 0.0f)
        {
            if (healthTreeLvl > 3) currentHp += upgradedDeadlineHeal;
            else currentHp += deadlineHeal;

            if (currentHp >= deathlineHp)
            {
                currentHp = deathlineHp;
                lastHitTime = 0.0f;
                if (healingFromDeathline)
                {
                    material.SetColor(1, 1, 1, 1);
                    healingFromDeathline = false;
                }
            }
            else
            {
                lastHitTime = 1.0f; // heal each second
                if (!healingFromDeathline)
                {
                    material.SetColor(0, 1, 0, 1);
                    healingFromDeathline = true;
                }
            }
        }
    }
    if (deathTime > 0.0f)
    {
        deathTime -= dt;
        if (deathTime <= 0.0f) Scene::LoadScene("LoseMenu.HScene");
    }
    if (inmunityTime > 0.0f)
    {
        inmunityTime -= dt;
    }
    if (reducedDamageTime > 0.0f)
    {
        reducedDamageTime -= dt;
    }
    if (blinkTime > 0.0f)
    {
        blinkTime -= dt;
        if (blinkTime <= 0.0f)
        {
            material.SetColor(1, 1, 1, 1);
            blinkTime = 0.0f;
        }
        else if (blinkTime < 0.15f)
        {
            if (positiveBlink)
            {
                material.SetColor(0, 1, 0, 1);
            }
            else
            {
                material.SetColor(1, 0, 0, 1);
            }
        }
        else if (blinkTime < 0.3f)
        {
            material.SetColor(1, 1, 1, 1);
        }
    }
    
    // power ups
    if (speedPowerUp > 0.0f)
    {
        speedPowerUp -= dt;
        if (speedPowerUp <= 0.0f) speedPowerUp = 0.0f;
    }
    if (fireratePowerUp > 0.0f)
    {
        fireratePowerUp -= dt;
        if (fireratePowerUp <= 0.0f) fireratePowerUp = 0.0f;
    }
    if (slowTimePowerUp > 0.0f)
    {
        slowTimePowerUp -= dt;
        if (slowTimePowerUp <= 0.0f)
        {
            slowTimePowerUp = 0.0f;
            Time::ChangeTimeScale(1.0f);
        }
    }
}

void PlayerStats::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "EnemyDrop")
    {
        EnemyDrop* enemyDrop = (EnemyDrop*)other.GetGameObject().GetScript("EnemyDrop");

        switch (enemyDrop->dropIndex)
        {
        case 0: // laser ammo
            GetAmmo(1, 0.4f * maxLaserAmmo);
            Audio::Event("w_ammo_pick");
            break;
        case 1: // first aid kit
            if (healthTreeLvl > 3) Heal(upgradedAidKitHeal);
            else Heal(aidKitHeal);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            GetPowerUp(enemyDrop->dropIndex - 2);
            break;
        default:
            Console::Log("Enemy Drop with wrong index.");
            break;
        }

        enemyDrop->Destroy();
    }
    else if (detectionTag == "Casette")
    {
        IndexContainer* indexContainer = (IndexContainer*)other.GetGameObject().GetScript("IndexContainer");
        if (!indexContainer)
        {
            Console::Log("Casette needs IndexContainer Script.");
            return;
        }
        if (!storage)
        {
            Console::Log("Storage missing in PlayerStats Script.");
            return;
        }

        /*switch (casettesPicked)
        {
        case 0:
            Audio::Event(storage->playAudio1.c_str());
            break;
        case 1:
            Audio::Event(storage->playAudio2.c_str());
            break;
        case 2:
            Audio::Event(storage->playAudio3.c_str());
            break;
        default:
            break;
        }*/

        switch (indexContainer->index)
        {
        case 1:
            storage->casette1Picked = true;
            storage->SaveData();
            if (CassetePicked == false)
            {
                CassetePicked = true;
                Audio::Event("info_alert");
            }
            break;
        case 2:
            storage->casette2Picked = true;
            storage->SaveData();
            if (CassetePicked == false)
            {
                CassetePicked = true;
                Audio::Event("info_alert");
            }
            break;
        case 3:
            storage->casette3Picked = true;
            storage->SaveData();
            if (CassetePicked == false)
            {
                CassetePicked = true;
                Audio::Event("info_alert");
            }
            break;
        default:
            Console::Log("Casette index only can be 1, 2 or 3.");
            break;
        }
        indexContainer->StopParticles();
        other.GetGameObject().SetActive(false);
    }
    else if (detectionTag == "CheckPoint")
    {
        if (!storage)
        {
            Console::Log("Storage missing in PlayerStats Script.");
            return;
        }

        storage->SaveData();
    }
}

void PlayerStats::TakeDamage(float amount, float resistanceDamage)
{
    if (inmunityTime > 0.0f || currentHp <= 0) return; // only VS2

    if (reducedDamageTime > 0.0f)
    {
        amount *= (1.0f - reducedDamageTime);
        // 1.0 - 0.5 = 0.5 --> 50% max damage reduction
        // 0.5 --> reduced damage duration
    }
    else
    {
        reducedDamageTime = 0.5f;
    }

    float shieldBefore = shield;
    shield -= amount;
    if (shield <= 0.0f)
    {
        currentHp += shield;
        shield = 0.0f;
        if (shieldBefore > 0 && hudPowerUp) hudPowerUp->RemovePowerUp(PowerUp_Type::SHIELD);
    }

    // hp damage
    if (currentHp <= 0)
    {
        if (secondLife)
        {
            secondLife = false;
            currentHp = 1;
            Audio::Event("starlord_damaged"); // second life audio
        }
        else
        {
            currentHp = 0;
            Audio::Event("starlord_dead");
            Audio::Event("game_over");
            deathTime = 1.5f;
            if (playerMove) playerMove->PlayDeathAnim();
            return;
        }
    }
    else
    {
        Audio::Event("starlord_damaged");
        blinkTime = 0.5f;
        positiveBlink = false;
        material.SetColor(1, 0, 0, 1);
        if (cam != nullptr) 
        {
            cam->Shake(0.1f, 0.5f);
        }

    }

    lastHitTime = 3.0f; // 3 seg to auto heal after a hit
    if (healingFromDeathline)
    {
        material.SetColor(1, 1, 1, 1);
        healingFromDeathline = false;
    }
}

void PlayerStats::Heal(float amount)
{
    currentHp += amount;
    blinkTime = 0.5f;
    positiveBlink = true;
    material.SetColor(0, 1, 0, 1);

    Audio::Event("heal");

    if (currentHp > currentMaxHp) currentHp = currentMaxHp;
}

int PlayerStats::GetAmmonByType(int type)
{
    switch (type)
    {
    case 0:
        return 1; // infinite base ammo
        break;
    case 1:
        return laserAmmo;
        break;
    case 2:
        return specialAmmo;
        break;
    default:
        Console::Log("Invalid type, type can only be 0, 1 or 2.");
        return -1;
        break;
    }
}

void PlayerStats::GetAmmo(int type, int amount)
{
    switch (type)
    {
    case 1:
        //script Municion
        if (ammo_Script) ammo_Script->opacity_Active = true;
        laserAmmo += amount;
        if (laserAmmo > maxLaserAmmo) laserAmmo = maxLaserAmmo;
        break;
    case 2:
        specialAmmo += amount;
        if (specialAmmo > maxSpecialAmmo) specialAmmo = maxSpecialAmmo;
        break;
    default:
        Console::Log("Invalid type, can only get ammo of types 1 or 2.");
        break;
    }
}

void PlayerStats::UseAmmo(int type, int amount)
{
    switch (type)
    {
    case 1:
        laserAmmo -= amount;
        break;
    case 2:
        specialAmmo -= amount;
        break;
    default:
        Console::Log("Invalid type, can only use ammo of types 1 or 2.");
        break;
    }
}

void PlayerStats::UpgradeTreeLvl(int tree)
{
    switch (tree)
    {
    case 0:
        movementTreeLvl++;
        break;
    case 1:
        armoryTreeLvl++;
        break;
    case 2:
        healthTreeLvl++;
        break;
    case 3:
        specialTreeLvl++;
        break;
    default:
        Console::Log("Invalid tree index, can only be 0, 1, 2 or 3.");
        break;
    }
}

void PlayerStats::GetPowerUp(int index)
{
    if (hudPowerUp == nullptr) return;

    switch (index)
    {
    case 0:
        speedPowerUp = 5.0f;
        hudPowerUp->AddPowerUp(PowerUp_Type::SPEED_INCREASE, speedPowerUp);
        hudPowerUp_Alert->Reset = true;
        hudPowerUp_Alert->Swap_PowerUp_Texture(0);
        break;
    case 1:
        fireratePowerUp = 5.0f;
        hudPowerUp->AddPowerUp(PowerUp_Type::FIRERATE_INCREASE, fireratePowerUp);
        hudPowerUp_Alert->Reset = true;
        hudPowerUp_Alert->Swap_PowerUp_Texture(1);
        break;
    case 2:
        shield = 50.0f;
        hudPowerUp->AddPowerUp(PowerUp_Type::SHIELD, 1);
        hudPowerUp_Alert->Reset = true;
        hudPowerUp_Alert->Swap_PowerUp_Texture(2);
        break;
    case 3:
        GetAmmo(1, maxLaserAmmo);
        GetAmmo(2, maxSpecialAmmo);
        hudPowerUp_Alert->Reset = true;
        hudPowerUp_Alert->Swap_PowerUp_Texture(3);
        break;
    case 4:
        slowTimePowerUp = 5.0f;
        Time::ChangeTimeScale(0.5f);
        hudPowerUp->AddPowerUp(PowerUp_Type::SLOW_TIME, slowTimePowerUp);
        hudPowerUp_Alert->Reset = true;
        hudPowerUp_Alert->Swap_PowerUp_Texture(4);
        break;
    default:
        Console::Log("Invalid powe up index, can only be 0, 1, 2 or 3.");
        break;
    }

    Audio::Event("power_up");
}

void PlayerStats::SaveInStorage(int index)
{
    if (!storage)
    {
        Console::Log("Storage missing in PlayerStats Script.");
        return;
    }

    switch (index)
    {
    case 0:
        storage->upgradeBlueprintAmount++;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        storage->unlockGunBlueprint = index;
        break;
    
    default:
        break;
    }

    storage->SaveData();
}

void PlayerStats::SaveChestData(int chestContent, int chestIndex, bool saveChest)
{
    if (saveChest) storage->SaveDataFromChest(chestIndex, chestContent);

    SaveInStorage(chestContent);
}