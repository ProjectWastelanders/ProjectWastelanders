#include "OpenMenuInterruptor.h"
#include "../Player/PlayerMove.h"
#include "../UI Test folder/HUB/HUB_UIManager.h"
HELLO_ENGINE_API_C OpenMenuInterruptor* CreateOpenMenuInterruptor(ScriptToInspectorInterface* script)
{
    OpenMenuInterruptor* classInstance = new OpenMenuInterruptor();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Number of menus", &classInstance->numberOfAsignedMenus);
    script->AddDragBoxGameObject("Menu to Open 1", &classInstance->menu[0]);
    script->AddDragBoxGameObject("Menu to Open 2", &classInstance->menu[1]);
    script->AddDragBoxGameObject("Menu to Open 3", &classInstance->menu[2]);
    script->AddDragBoxGameObject("Menu to Open 4", &classInstance->menu[3]);
    script->AddDragBoxGameObject("Menu to Open 5", &classInstance->menu[4]);
    script->AddDragBoxGameObject("Menu to Open 6", &classInstance->menu[5]);
    script->AddDragBoxGameObject("Menu to Open 7", &classInstance->menu[6]);
    script->AddDragBoxGameObject("Menu to Open 8", &classInstance->menu[7]);
    script->AddDragBoxGameObject("Menu to Open 9", &classInstance->menu[8]);
    script->AddDragBoxGameObject("Menu to Open 10", &classInstance->menu[9]);
    script->AddDragBoxGameObject("Menu Panel", &classInstance->menuPanel);
    script->AddDragBoxGameObject("Button Guide", &classInstance->buttonGuide);
    return classInstance;
}

void OpenMenuInterruptor::Start()
{

}

void OpenMenuInterruptor::Update()
{

}

void OpenMenuInterruptor::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        buttonGuide.SetActive(true);
    }
}

void OpenMenuInterruptor::OnCollisionStay(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            //playerMove = (PlayerMove*)other.GetGameObject().GetScript("PlayerMove");
            //if (playerMove != nullptr) playerMove->openingChest = true;
            OpenMenus();

        }
    }
}

void OpenMenuInterruptor::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        buttonGuide.SetActive(false);
    }
}

void OpenMenuInterruptor::OpenMenus()
{
    if (HUB_UIManager::IsPanelOpened())
        return;
    for (size_t i = 0; i < numberOfAsignedMenus; i++)
    {
        menu[i].SetActive(true);
    }
    HUB_UIManager::OpenPanel();
}