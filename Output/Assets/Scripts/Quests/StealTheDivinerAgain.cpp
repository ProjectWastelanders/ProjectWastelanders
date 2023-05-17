#include "StealTheDivinerAgain.h"
#include "../Player/PlayerStorage.h"
#include "ReachTheSpaceship.h"
HELLO_ENGINE_API_C StealTheDivinerAgain* CreateStealTheDivinerAgain(ScriptToInspectorInterface* script)
{
    StealTheDivinerAgain* classInstance = new StealTheDivinerAgain();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Final Text Panel", &classInstance->finalText);
    script->AddDragBoxGameObject("Reach The Spaceship GO", &classInstance->reachTheSpaceShipGO);
    script->AddCheckBox("aaaaaaa", &classInstance->uwu);

    return classInstance;
}

void StealTheDivinerAgain::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in StealTheDivinerAgain Script.");

    reachTheSpaceShip = (ReachTheSpaceship*)reachTheSpaceShipGO.GetScript("ReachTheSpaceship");
    if (reachTheSpaceShip == nullptr) Console::Log("ReachTheSpaceship missing in StealTheDivinerAgain Script.");


    Game::FindGameObjectsWithTag("Camera", &camObj, 1);
    cam = (CamMov*)camObj.GetScript("CamMov");
    if (cam == nullptr) Console::Log("CamMov missing in StealTheDivinerAgain Script.");
}

void StealTheDivinerAgain::Update()
{
    //if (Input::GetKey(KeyCode::KEY_J) == KeyState::KEY_DOWN) if (cam) cam->Earthquake(3), uwu = true;
   
}

void StealTheDivinerAgain::OnCollisionStay(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            if (playerStorage)
            {
                playerStorage->skillPoints += 3;
                playerStorage->SaveData();
            }
            API_QuickSave::SetBool("level3_halfCompleted", true);
            API_QuickSave::SetBool("IsInMiddleOfLevel", false);
            finalText.SetActive(true);
            if (reachTheSpaceShip) reachTheSpaceShip->EnableMision();
            if (cam) cam->Earthquake(4), uwu = true;
            gameObject.SetActive(false);
        }
    }
}