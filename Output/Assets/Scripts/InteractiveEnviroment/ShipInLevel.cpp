#include "ShipInLevel.h"
#include "../FadeToBlack.h"
#include "../Player/PlayerStorage.h"
#include "ControlPanelShip.h"

HELLO_ENGINE_API_C ShipInLevel* CreateShipInLevel(ScriptToInspectorInterface* script)
{
	ShipInLevel* classInstance = new ShipInLevel();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
	script->AddDragInt("LVL Index", &classInstance->lvlIndex);
	script->AddDragBoxTransform("Position To Return", &classInstance->positionToReturn);
	script->AddDragBoxGameObject("Player", &classInstance->playerGO);
	script->AddDragBoxGameObject("ControlPanelShipGO", &classInstance->controlPanelShipGO);

	return classInstance;
}


void ShipInLevel::Init()
{
	gameObject.SetActive(true);
}

void ShipInLevel::Start()
{

	fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
	if (!fadeToBlackRef) Console::Log("fadeToBlackRef Missing in Shipinlevel.");

	controlPanelShipRef = (ControlPanelShip*)controlPanelShipGO.GetScript("ControlPanelShip");
	if (!controlPanelShipRef) Console::Log("fadeToBlackRef Missing in StartingShipinlevel.");

	playerStorageRef = (PlayerStorage*)playerGO.GetScript("PlayerStorage");
	if (!playerStorageRef) Console::Log("playerStorageRef Missing in Shipinlevel.");

	if (lvlIndex > 5)
	{
		lvlIndex = 1;
	}

	if (playerStorageRef)
		playerStorageRef->levelIndex = lvlIndex;

	if (API_QuickSave::GetBool("IsInMiddleOfLevel") == true)
	{
		if (controlPanelShipRef)
			controlPanelShipRef->ShipActivated = true;
		
	}
	else
	{
		gameObject.SetActive(false);
	}
	

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
		API_QuickSave::SetFloat("PlayerIndicatorPosX", -0.7f);
		API_QuickSave::SetFloat("PlayerIndicatorPosY", -0.39f);
		API_QuickSave::SetBool("level1_completed", true);
		//API_QuickSave::SetBool("IsInMiddleOfLevel", false);
	}

}

void ShipInLevel::OnCollisionStay(API::API_RigidBody other)
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
				API_QuickSave::SetBool("IsInMiddleOfLevel", true);
				
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
