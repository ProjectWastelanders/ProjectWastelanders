#include "StartingShipOnLevel.h"
#include "../FadeToBlack.h"
#include "../Player/PlayerStorage.h"
#include "ControlPanelShip.h"

HELLO_ENGINE_API_C StartingShipOnLevel* CreateStartingShipOnLevel(ScriptToInspectorInterface* script)
{
	StartingShipOnLevel* classInstance = new StartingShipOnLevel();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
	script->AddDragInt("LVL Index", &classInstance->lvlIndex);
	script->AddDragBoxTransform("Position To Return", &classInstance->positionToReturn);
	script->AddDragBoxGameObject("Player", &classInstance->playerGO);
	script->AddDragBoxGameObject("ControlPanelShipGO", &classInstance->controlPanelShipGO);

	return classInstance;
}

void StartingShipOnLevel::Init()
{
	gameObject.SetActive(true);
}

void StartingShipOnLevel::Start()
{
	fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
	if (!fadeToBlackRef) Console::Log("fadeToBlackRef Missing in StartingShipinlevel.");

	controlPanelShipRef = (ControlPanelShip*)controlPanelShipGO.GetScript("ControlPanelShip");
	if (!controlPanelShipRef) Console::Log("fadeToBlackRef Missing in StartingShipinlevel.");

	playerStorageRef = (PlayerStorage*)playerGO.GetScript("PlayerStorage");
	if (!playerStorageRef) Console::Log("playerStorageRef Missing in StartingShipinlevel.");

	if (lvlIndex > 5)
	{
		lvlIndex = 1;
	}

	if(playerStorageRef)
		playerStorageRef->levelIndex = lvlIndex;

	
	if (API_QuickSave::GetBool("IsInMiddleOfLevel") == true)
	{
		if (controlPanelShipRef)
			controlPanelShipRef->ShipActivated = false;
	}
}
void StartingShipOnLevel::Update()
{
	
}

void StartingShipOnLevel::OnCollisionEnter(API::API_RigidBody other)
{
}

void StartingShipOnLevel::OnCollisionStay(API::API_RigidBody other)
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
		else if (API_QuickSave::GetBool("ComesFromHub"))
		{
			fadeToBlackRef->blackToFade = true;

			API_QuickSave::SetBool("ComesFromHub", false);


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

void StartingShipOnLevel::OnCollisionExit(API::API_RigidBody other)
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
	{
		fadeToBlackRef->SetOpacity(1.0f);
		fadeToBlackRef->blackToFade = true;
		timerTotp = 0.0f;
	}
}