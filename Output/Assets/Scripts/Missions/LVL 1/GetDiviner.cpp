#include "GetDiviner.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C GetDiviner* CreateGetDiviner(ScriptToInspectorInterface* script)
{
    GetDiviner* classInstance = new GetDiviner();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Final Text Panel", &classInstance->finalText);

    script->AddDragBoxGameObject("Press Button", &classInstance->PressButton);
    return classInstance;
}

void GetDiviner::Start()
{
    PressButton.SetActive(false);
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");
}

void GetDiviner::Update()
{

}

void GetDiviner::OnCollisionStay(API_RigidBody other)
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
            API_QuickSave::SetBool("level1_completed", true);
            API_QuickSave::SetBool("IsInMiddleOfLevel", false);
            Scene::LoadScene("SpaceshipHUB_Scene.HScene");
        }
        
    }
}
void GetDiviner::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        PressButton.SetActive(true);
    }
}

void GetDiviner::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        PressButton.SetActive(false);
    }
}