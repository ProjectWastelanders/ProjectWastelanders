#include "ShipInLevel.h"
#include "../FadeToBlack.h"

HELLO_ENGINE_API_C ShipInLevel* CreateShipInLevel(ScriptToInspectorInterface* script)
{
	ShipInLevel* classInstance = new ShipInLevel();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
	script->AddDragFloat("timer", &classInstance->timerTotp);
	script->AddDragBoxTransform("Position To Return", &classInstance->positionToReturn);

	return classInstance;
}

void ShipInLevel::Start()
{

	fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
	if (!fadeToBlackRef) Console::Log("fadeToBlackRef Missing in Shipinlevel.");
}
void ShipInLevel::Update()
{



}

void ShipInLevel::OnCollisionEnter(API::API_RigidBody other)
{

	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionTag == "Player")
	{
		API_QuickSave::SetFloat("PlayerPosX", positionToReturn.GetGlobalPosition().x );
		API_QuickSave::SetFloat("PlayerPosY", positionToReturn.GetGlobalPosition().y);
		API_QuickSave::SetFloat("PlayerPosZ", positionToReturn.GetGlobalPosition().z);
		//API_QuickSave::SetFloat("PlayerIndicatorPosX", -0.7f);
		//API_QuickSave::SetFloat("PlayerIndicatorPosY", -0.39f);
		API_QuickSave::SetBool("level1_completed", true);
		
	}

}

void ShipInLevel::OnCollisionStay(API::API_RigidBody other)
{

    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
		if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
		{
			fadeToBlackRef->fadeToBlack = true;

		}
		if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
		{
			timerTotp += Time::GetDeltaTime();

			if (timerTotp >= 1.2f)
			{

				Scene::LoadScene("SpaceshipHUB_Scene.HScene");

				timerTotp = 0.0f;
			}
		}
    }

}
