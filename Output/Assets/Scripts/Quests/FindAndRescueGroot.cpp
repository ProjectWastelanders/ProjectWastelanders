#include "FindAndRescueGroot.h"
#include "../Player/PlayerStorage.h"
#include "../FadeToBlack.h"

HELLO_ENGINE_API_C FindAndRescueGroot* CreateFindAndRescueGroot(ScriptToInspectorInterface* script)
{
    FindAndRescueGroot* classInstance = new FindAndRescueGroot();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
    return classInstance;
}

void FindAndRescueGroot::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in FindAndRescue Script.");

    fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
    if (!fadeToBlackRef) Console::Log("fadeToBlackRef Missing in Shipinlevel.");
}
void FindAndRescueGroot::Update()
{

}

void FindAndRescueGroot::OnCollisionStay(API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Player")
    {

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
        {
            fadeToBlackRef->fadeToBlack = true;

            if (playerStorage)
            {
                playerStorage->skillPoints += 3;
                playerStorage->SaveData();
            }
            API_QuickSave::SetBool("level2_completed", true);

        }
        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
        {
            timerToTp += Time::GetDeltaTime();

            if (timerToTp >= 1.2f)
            {

                Scene::LoadScene("SpaceshipHUB_Scene.HScene");
                timerToTp = 0.0f;
            }
        }

    }
}