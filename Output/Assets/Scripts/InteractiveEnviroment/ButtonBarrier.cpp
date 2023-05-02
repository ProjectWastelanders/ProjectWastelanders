#include "ButtonBarrier.h"
HELLO_ENGINE_API_C ButtonBarrier* CreateButtonBarrier(ScriptToInspectorInterface* script)
{
    ButtonBarrier* classInstance = new ButtonBarrier();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void ButtonBarrier::Start()
{

}

void ButtonBarrier::Update()
{

}

void ButtonBarrier::OnCollisionStay(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        if (picked) return;

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
        {
            picked = true;
        }
    }
}