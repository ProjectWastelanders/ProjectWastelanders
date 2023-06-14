#include "ShipOnOtherLevels.h"
#include "../Player/PlayerStorage.h"
#include "../FadeToBlack.h"

HELLO_ENGINE_API_C ShipOnOtherLevels* CreateShipOnOtherLevels(ScriptToInspectorInterface* script)
{
	ShipOnOtherLevels* classInstance = new ShipOnOtherLevels();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
	script->AddDragInt("LVL Index", &classInstance->lvlIndex);
	script->AddDragBoxTransform("Position To Return", &classInstance->positionToReturn);
	script->AddDragBoxGameObject("Player", &classInstance->playerGO);

	return classInstance;
}

void ShipOnOtherLevels::Start()
{
	fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
	if (!fadeToBlackRef) Console::Log("fadeToBlackRef Missing in ShipOnOtherLevels.");

	playerStorageRef = (PlayerStorage*)playerGO.GetScript("PlayerStorage");
	if (!playerStorageRef) Console::Log("playerStorageRef Missing in ShipOnOtherLevels.");

	if (lvlIndex > 5)
	{
		lvlIndex = 1;
	}

	playerStorageRef->levelIndex = lvlIndex;
}

void ShipOnOtherLevels::Update()
{

}

void ShipOnOtherLevels::OnCollisionEnter(API::API_RigidBody other)
{
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionTag == "Player")
	{
		API_QuickSave::SetFloat("PlayerPosX", positionToReturn.GetGlobalPosition().x);
		API_QuickSave::SetFloat("PlayerPosY", positionToReturn.GetGlobalPosition().y);
		API_QuickSave::SetFloat("PlayerPosZ", positionToReturn.GetGlobalPosition().z);
		API_QuickSave::SetFloat("PlayerIndicatorPosX", -0.7f);
		API_QuickSave::SetFloat("PlayerIndicatorPosY", -0.39f);
		//API_QuickSave::SetBool("IsInMiddleOfLevel", false);
	}
}

void ShipOnOtherLevels::OnCollisionStay(API::API_RigidBody other)
{
	std::string detectionTag = other.GetGameObject().GetTag();
	if (detectionTag == "Player")
	{

		if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
		{
			timerTotp += Time::GetDeltaTime();

			if (timerTotp >= 0.98f)
			{

				Scene::LoadScene("SpaceshipHUB_Scene.HScene");

			}
			else
			{
				fadeToBlackRef->fadeToBlack = true;
			}
		}
		else if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP)
		{
			if (timerTotp <= 0.98f)
			{
				fadeToBlackRef->fadeToBlack = false;
				fadeToBlackRef->blackToFade = true;
				timerTotp = 0.0f;
			}

		}
	}
}

void ShipOnOtherLevels::OnCollisionExit(API::API_RigidBody other)
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
	{
		fadeToBlackRef->SetOpacity(1.0f);
		fadeToBlackRef->blackToFade = true;
		timerTotp = 0.0f;
	}
}
