#include "Chest.h"

HELLO_ENGINE_API_C Chest* CreateChest(ScriptToInspectorInterface* script)
{
    Chest* classInstance = new Chest();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxAnimationPlayer("Chest Animator Player", &classInstance->chestAnimatorPlayer);
    script->AddDragFloat("Open Chest Time", &classInstance->maxOpenChestTime);
    script->AddCheckBox("Tutorial Special Weapon", &classInstance->tutorialSpecialWeapon);
    script->AddCheckBox("Tutorial Weapon Blueprint", &classInstance->tutorialWeaponBlueprint);
    script->AddDragInt("Chest Index", &classInstance->chestIndex);
    script->AddDragInt("Item Index", &classInstance->itemIndex);
    script->AddCheckBox("Can Get Special Gun", &classInstance->canGetGun);
    script->AddCheckBox("Save Chest Interaction", &classInstance->saveChest);

    script->AddDragBoxUIImage("Guide Button", &classInstance->guideButton);

    script->AddDragBoxUIImage("Tutorial_Img", &classInstance->Tutorial_Img);

    script->AddCheckBox("Blueprint_Tutorial", &classInstance->bluprintTutorial);

    return classInstance;
}

void Chest::Start()
{
    openChestTime = maxOpenChestTime;
    //openChestTimeBar = 0.0f;
    opening = false;
    guideButton.GetGameObject().SetActive(false);
    //guideButton.FillImage(1);
    initalPos = { -1.250, -0.700, 0 };
    movingPos = { -1.250, -0.700, 0 };
    Tutorial_Img.GetGameObject().GetTransform().SetPosition(initalPos);
    Tutorial_Img.GetGameObject().SetActive(false);
    finalPos = { -0.780, -0.700, 0 };
}

void Chest::Update()
{
   
    if (opening)
    {
        openChestTime -= Time::GetRealTimeDeltaTime();
        openChestTimeBar += Time::GetRealTimeDeltaTime();
        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_UP)
        {
            if (playerMove) playerMove->StopOpenChestAnim();
            openChestTime = maxOpenChestTime;
            openChestTimeBar = 0;
            opening = false;
        }
        if (openChestTime <= 0.0f)
        {
            opening = false;
            guideButton.GetGameObject().SetActive(false);

            if (!playerGunManager || !playerStats || !playerMove)
            {
                openChestTime = maxOpenChestTime;
                return;
            }

            switch (itemIndex)
            {
            case 0: // Upgrade Blueprint
                playerStats->SaveChestData(itemIndex, chestIndex, saveChest);
                if (playerStats->storage->hud_blueprints) playerStats->storage->hud_blueprints->UpgradeAlert(itemIndex);
                if (playerStats->storage->hud_Alert_Prints) playerStats->storage->hud_Alert_Prints->Swap_BluePrint_Texture(itemIndex);
                break;
            case 1: // Unlock Gun
            case 2:
            case 3:
            case 4:
                playerStats->SaveChestData(itemIndex, chestIndex, saveChest);
                if (playerStats->storage->hud_blueprints) playerStats->storage->hud_blueprints->New_WeaponAlert(itemIndex);
                if (playerStats->storage->hud_Alert_Prints) playerStats->storage->hud_Alert_Prints->Swap_BluePrint_Texture(itemIndex);
                break;
            case 5: // Get Flamethrower
                playerGunManager->GetGun(3, 5);
                playerStats->specialAmmo = 600;
                playerStats->maxSpecialAmmo = 600;
                playerStats->SaveChestData(6, chestIndex, saveChest); // save game
                if (playerStats->storage->hud_blueprints) playerStats->storage->hud_blueprints->Special_WeaponAlert(5);
                if (playerStats->storage->hud_Alert_Prints) playerStats->storage->hud_Alert_Prints->Swap_BluePrint_Texture(5);
                break;
            case 6: // Get Ricochet
                playerGunManager->GetGun(3, 6);
                playerStats->specialAmmo = 50;
                playerStats->maxSpecialAmmo = 50;
                playerStats->SaveChestData(7, chestIndex, saveChest); // save game
                if (playerStats->storage->hud_blueprints) playerStats->storage->hud_blueprints->Special_WeaponAlert(6);
                if (playerStats->storage->hud_Alert_Prints) playerStats->storage->hud_Alert_Prints->Swap_BluePrint_Texture(6);
                break;
            default:
                Console::Log("Item Index is not between 0 and 7.");
                break;
            }
            
            Audio::Event("open_chest");
            playerMove->StopOpenChestAnim();
            chestAnimatorPlayer.Play();
            opened = true;
            if (endTutorial == true || bluprintTutorial == false)
            {
                Tutorial_Img.GetGameObject().SetActive(false);
                gameObject.SetActive(false);
            }
            if(tutorialSpecialWeapon == true || tutorialWeaponBlueprint)Audio::Event("info_alert");
            activeTutorial = true;

        }
    }
    //guideButton.FillImage(openChestTime/maxOpenChestTime);


    if (activeTutorial == true && endTutorial == false && hideChest == false)
    {
        Tutorial_Img.GetGameObject().SetActive(true);
        if (Tutorial_Img.GetGameObject().GetTransform().GetLocalPosition().x < finalPos.x)
        {
            movingPos.x += 0.32 * Time::GetDeltaTime();
            timerTutorial = true;
           
        }
        if (timerTutorial == true)
        {
            showTutorial += 1.0f * Time::GetDeltaTime();
            
        }
        
    }

    if (showTutorial >=  8.0f) 
    {
       
        if (Tutorial_Img.GetGameObject().GetTransform().GetLocalPosition().x > initalPos.x)
        {
            movingPos.x -= 0.32 * Time::GetDeltaTime();
            timerTutorial = false;
            hideChest = true;
        }

        else if(Tutorial_Img.GetGameObject().GetTransform().GetLocalPosition().x < initalPos.x) {
            endTutorial = true;
        }

        

    }
    Tutorial_Img.GetGameObject().GetTransform().SetPosition(movingPos);
}

void Chest::OnCollisionStay(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (opening) return;

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");
            if (playerMove == nullptr) return;

            playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
            playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
            if (playerMove) playerMove->PlayOpenChestAnim();
            opening = true;

            
           //float distanceX = gameObject.GetTransform().GetGlobalPosition().x - other.GetGameObject().GetTransform().GetGlobalPosition().x;
            //float distanceZ = gameObject.GetTransform().GetGlobalPosition().z - other.GetGameObject().GetTransform().GetGlobalPosition().z;
            
            /*
            if (abs(distanceX) < abs(distanceZ))
            {
                if (distanceZ >= 0.0f && playerMove->aimAngle <= 90 && playerMove->aimAngle > -90) // chest up
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
                else if (distanceZ < 0.0f && playerMove->aimAngle <= -90 && playerMove->aimAngle > -270) // chest down
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
            }
            else
            {
                if (distanceX < 0.0f && playerMove->aimAngle <= 0 && playerMove->aimAngle > -180) // chest left
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
                else if (distanceX >= 0.0f && (playerMove->aimAngle <= -180 || (playerMove->aimAngle < 90 && playerMove->aimAngle >= 0))) // chest right
                {
                    playerGunManager = (PlayerGunManager*)other.GetGameObject().GetScript("PlayerGunManager");
                    playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

                    if (playerMove) playerMove->PlayOpenChestAnim();
                    opening = true;
                }
            }*/
        }
        
    }
}

void Chest::OpenChestOnStart()
{
    chestAnimatorPlayer.Play();
    gameObject.SetActive(false);
    opened = true;
}

void Chest::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        guideButton.GetGameObject().SetActive(true);
    }
}

void Chest::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
       guideButton.GetGameObject().SetActive(false);
       
    }
}