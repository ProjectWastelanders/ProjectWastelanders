#include "TP_Cabin.h"
#include "../Player/PlayerStats.h"
#include "../../Game/Prefabs/Environment/TP_Sphere/TpRotateSphere.h"

HELLO_ENGINE_API_C TP_Cabin* CreateTP_Cabin(ScriptToInspectorInterface* script)
{
	TP_Cabin* classInstance = new TP_Cabin();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxTransform("Destination: ", &classInstance->destination);
	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);
	script->AddDragBoxPrefabResource("Big Sphere Prefab", &classInstance->big_effectSpherePrefab);
	script->AddDragBoxPrefabResource("Small Sphere Prefab", &classInstance->small_effectSpherePrefab);
	script->AddDragFloat("Scale Value Big", &classInstance->scaleValue_big);
	script->AddDragFloat("Scale Value Small", &classInstance->scaleValue_small);

	return classInstance;
}

void TP_Cabin::Start()
{
	destinationSmoke = destination.GetGameObject().AddParticleSystem(smoke);

	Game::FindGameObjectsWithTag("Player", &playerGO, 1);

	playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
	if (playerStats == nullptr) Console::Log("Player is missing in TP CABIN Script.");

	hasSpawnedSphere = false;

	startParticles = 0.8f;
	endParticles = 1.1f;
	tpTime = 1.2f;

	canTp = true;
	enableCanTp = false;
}

void TP_Cabin::Update()
{
	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP)
	{
		canTp = true;
	}

	if (canTp == false)
	{
		sphereGrowingTime -= Time::GetDeltaTime();
	}
	else if (Input::GetGamePadButton(GamePadButton::BUTTON_X) != KeyState::KEY_REPEAT)
	{
		sphereGrowingTime -= Time::GetDeltaTime();
	}

	if (timeHoldButton < 0.0f)
	{
		timeHoldButton = 0.0f;
	}

	if (sphereGrowingTime < 0)
	{
		sphereGrowingTime = 0;

	}
	else if (sphereGrowingTime > tpTime)
	{
		sphereGrowingTime = tpTime;
	}

	sphereGrowing = sphereGrowingTime / tpTime;

	if (sphereGrowing <= 0)
	{
		DestroySphere();
	}
	else
	{
		SpawnSphere();
	}
}

void TP_Cabin::SpawnSphere()
{
	if (hasSpawnedSphere == false)
	{
		effectSphere = Game::InstancePrefab(big_effectSpherePrefab, API_GameObject());
		effectSphere2 = Game::InstancePrefab(big_effectSpherePrefab, API_GameObject());

		childSphere = Game::InstancePrefab(small_effectSpherePrefab, API_GameObject());
		childSphere2 = Game::InstancePrefab(small_effectSpherePrefab, API_GameObject());

		rotateSphere1 = (TpRotateSphere*)childSphere.GetScript("TpRotateSphere");
		rotateSphere2 = (TpRotateSphere*)childSphere2.GetScript("TpRotateSphere");

		if (rotateSphere1)
		{
			rotateSphere1->timeCount = 0.0f;
			rotateSphere1->deformationTime = 0.0f;
			rotateSphere1->deformedVer = false;
			rotateSphere1->inInFirstPart = true;
			rotateSphere1->partsCount = 0;
		}
		if (rotateSphere2)
		{
			rotateSphere2->timeCount = 0.0f;
			rotateSphere2->deformationTime = 0.0f;
			rotateSphere2->deformedVer = false;
			rotateSphere2->inInFirstPart = true;
			rotateSphere2->partsCount = 0;
		}

		hasSpawnedSphere = true;
	}

	float tempScale = sphereGrowing * scaleValue_big;
	effectSphere.GetTransform().SetScale(tempScale, tempScale, tempScale);
	effectSphere2.GetTransform().SetScale(tempScale, tempScale, tempScale);
	tempScale = sphereGrowing * scaleValue_small;

	if (rotateSphere1) 
	{
		rotateSphere1->sphereSize = tempScale;
	}
	if (rotateSphere2)
	{
		rotateSphere2->sphereSize = tempScale;
	}

	effectSphere.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y + 1, gameObject.GetTransform().GetGlobalPosition().z);
	effectSphere2.GetTransform().SetPosition(destination.GetGlobalPosition().x, destination.GetGlobalPosition().y + 1, destination.GetGlobalPosition().z);
	childSphere.GetTransform().SetPosition(gameObject.GetTransform().GetGlobalPosition().x, gameObject.GetTransform().GetGlobalPosition().y + 1, gameObject.GetTransform().GetGlobalPosition().z);
	childSphere2.GetTransform().SetPosition(destination.GetGlobalPosition().x, destination.GetGlobalPosition().y + 1, destination.GetGlobalPosition().z);

}

void TP_Cabin::DestroySphere()
{
	if (hasSpawnedSphere == true)
	{ 
		effectSphere.Destroy();
		effectSphere2.Destroy();
		childSphere.Destroy();
		childSphere2.Destroy();
		rotateSphere1 = nullptr;
		rotateSphere2 = nullptr;
		hasSpawnedSphere = false;
	}
}

void TP_Cabin::OnCollisionExit(API_RigidBody other)
{
	std::string detectionTag = other.GetGameObject().GetTag();

	if (detectionTag == "Player")
	{
		canTp = false;
		enableCanTp = false;
		timeHoldButton = 0.0f;
	}
}


void TP_Cabin::OnCollisionStay(API_RigidBody other)
{

	std::string detectionTag = other.GetGameObject().GetTag();


	if (detectionTag == "Player")
	{

		//INSERT UI
		if (playerStats->inCombat)
		{
		}
		else
		{
			if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
			{
				enableCanTp = true;
			}

			if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP)
			{
				enableCanTp = false;
			}

			if (enableCanTp == true)
			{
				
				if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
				{
					timeHoldButton += Time::GetDeltaTime();
					sphereGrowingTime += Time::GetDeltaTime();
				}
				else
				{
					timeHoldButton -= Time::GetDeltaTime();
				}

				if (canTp == true)
				{
					if (timeHoldButton > startParticles && timeHoldButton < endParticles)
					{
						Audio::Event("teleport_1");
					}
					else if (timeHoldButton > tpTime)
					{
						other.GetGameObject().GetTransform().SetPosition(destination.GetGlobalPosition());
						timeHoldButton = 0.0f;
						smoke.StopEmitting();
						destinationSmoke.StopEmitting();
						Audio::Event("teleport_2");

						if (!playerStats->showedTpDialog)
						{
							playerStats->showTpDialog = true;
						}
						enableCanTp = false;
						canTp = false;
					}
				}
				else
				{
					timeHoldButton = 0.0f;
					smoke.StopEmitting();
					destinationSmoke.StopEmitting();
				}
			}
		}
	}
}

