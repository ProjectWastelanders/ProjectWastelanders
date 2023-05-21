#include "StealTheDivinerAgain.h"
#include "../Player/PlayerStorage.h"
#include "../Dialogs/Lvl_3/stealDivinerDialog.h"
HELLO_ENGINE_API_C StealTheDivinerAgain* CreateStealTheDivinerAgain(ScriptToInspectorInterface* script)
{
    StealTheDivinerAgain* classInstance = new StealTheDivinerAgain();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Final Text Panel", &classInstance->finalText);
    script->AddCheckBox("aaaaaaa", &classInstance->uwu);

    script->AddDragBoxGameObject("DialogGO", &classInstance->Dialog);

    return classInstance;
}

void StealTheDivinerAgain::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in StealTheDivinerAgain Script.");

    stealDiviner_Dialog = (stealDivinerDialog*)Dialog.GetScript("stealDivinerDialog");
    if (stealDiviner_Dialog == nullptr) Console::Log("stealDiviner_Dialog missing in StealTheDivinerAgain Script.");


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
            stealDiviner_Dialog->activeDialogs = true;
            if (playerStorage)
            {
                playerStorage->skillPoints += 3;
                playerStorage->SaveData();
            }
            API_QuickSave::SetBool("level3_halfCompleted", true);
            API_QuickSave::SetBool("IsInMiddleOfLevel", false);
            finalText.SetActive(true);
            if (cam) cam->Earthquake(3), uwu = true;            
            gameObject.SetActive(false);
        }
    }
}