#include "OpenMenuTrigger.h"
HELLO_ENGINE_API_C OpenMenuTrigger* CreateOpenMenuTrigger(ScriptToInspectorInterface* script)
{
	OpenMenuTrigger* classInstance = new OpenMenuTrigger();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Menu game object", &classInstance->menu);
	script->AddDragBoxGameObject("Secundary menu game object", &classInstance->secondaryMenu);

	script->AddDragBoxGameObject("Button to press game object", &classInstance->buttonToPressImage);


	return classInstance;
}

void OpenMenuTrigger::Start()
{

}
void OpenMenuTrigger::Update()
{
	if (!isPlayerInsideTrigger)
		return;

	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN && !menu.IsActive())
	{
		Input::HandleGamePadButton(GamePadButton::BUTTON_X);
		menu.SetActive(true);
		secondaryMenu.SetActive(true);
	}
}

void OpenMenuTrigger::OnCollisionStay(API_RigidBody other)
{
	std::string name = other.GetGameObject().GetName();

	if (name == "Player")
	{
		isPlayerInsideTrigger = true;
		buttonToPressImage.SetActive(true);
	}
}

void OpenMenuTrigger::OnCollisionExit(API_RigidBody other)
{
	std::string name = other.GetGameObject().GetName();

	if (name == "Player")
	{
		isPlayerInsideTrigger = false;
		buttonToPressImage.SetActive(false);
	}
}
