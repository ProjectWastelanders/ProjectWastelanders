#include "TP_Cabin.h"
#include "../Player/PlayerStats.h"

HELLO_ENGINE_API_C TP_Cabin* CreateTP_Cabin(ScriptToInspectorInterface* script)
{
	TP_Cabin* classInstance = new TP_Cabin();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxTransform("Destination: ", &classInstance->destination);
	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);

	return classInstance;
}

void TP_Cabin::Start()
{
	destinationSmoke = destination.GetGameObject().AddParticleSystem(smoke);
	
	Game::FindGameObjectsWithTag("Player", &playerGO, 1);

	playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
	if(playerStats == nullptr) Console::Log("Player is missing in TP CABIN Script.");
}
void TP_Cabin::Update()
{
}

void TP_Cabin::OnCollisionStay(API_RigidBody other)
{

	std::string detectionTag = other.GetGameObject().GetTag();


	if (detectionTag == "Player")
	{

		//INSERT UI
		if (playerStats->inCombat)
		{
			Console::Log("PLAYER BEING DETECTED");
		}
		else
		{

			Console::Log("PLAYER NOT BEING DETECTED");

			if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
			{
				timeHoldButton += Time::GetDeltaTime();
			}
			else
			{
				timeHoldButton -= Time::GetDeltaTime();
				smoke.StopEmitting();
				destinationSmoke.StopEmitting();
			}

			if (1.1f > timeHoldButton && timeHoldButton > 0.8f)
			{
				smoke.Play();
				destinationSmoke.Play();
				Audio::Event("teleport_1");
			}
			else if (timeHoldButton > 1.2f)
			{
				other.GetGameObject().GetTransform().SetPosition(destination.GetGlobalPosition());
				timeHoldButton = 0.0f;
				smoke.StopEmitting();
				destinationSmoke.StopEmitting();
				Audio::Event("teleport_2");

			}

		}
		
	}

}
