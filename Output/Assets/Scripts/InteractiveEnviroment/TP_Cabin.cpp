#include "TP_Cabin.h"
#include "../Player/PlayerStats.h"

HELLO_ENGINE_API_C TP_Cabin* CreateTP_Cabin(ScriptToInspectorInterface* script)
{
	TP_Cabin* classInstance = new TP_Cabin();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxTransform("Destination: ", &classInstance->destination);
	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);
	script->AddDragBoxPrefabResource("Sphere Prefab", &classInstance->effectSpherePrefab);
	script->AddDragFloat("Scale Value", &classInstance->scaleValue);
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
}

void TP_Cabin::Update()
{
	if (canTp == false)
	{
		sphereGrowingTime -= Time::GetDeltaTime();
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) != KeyState::KEY_REPEAT)
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
		//effectSphere = Game::InstancePrefab(effectSpherePrefab, API_GameObject());
		//effectSphere2 = Game::InstancePrefab(effectSpherePrefab, API_GameObject());
		effectSphere = Game::InstancePrefab(effectSpherePrefab, gameObject);
		effectSphere2 = Game::InstancePrefab(effectSpherePrefab, destination.GetGameObject());
		hasSpawnedSphere = true;
	}

	//effectSphere.GetTransform().SetScale(pow(timeHoldButton,scaleValue), pow(timeHoldButton, scaleValue), pow(timeHoldButton, scaleValue));
	effectSphere.GetTransform().SetScale(sphereGrowing * scaleValue, sphereGrowing * scaleValue, sphereGrowing * scaleValue);
	effectSphere2.GetTransform().SetScale(sphereGrowing * scaleValue, sphereGrowing * scaleValue, sphereGrowing * scaleValue);

	//effectSphere.GetTransform().SetPosition(playerGO.GetTransform().GetGlobalPosition().x, playerGO.GetTransform().GetGlobalPosition().y + 2, playerGO.GetTransform().GetGlobalPosition().z);
	//effectSphere2.GetTransform().SetPosition(playerGO.GetTransform().GetGlobalPosition().x, playerGO.GetTransform().GetGlobalPosition().y + 2, playerGO.GetTransform().GetGlobalPosition().z);
	
	/*API_GameObject newProjectile = Game::CreateGameObject("EnemyProjectile", "EnemyProjectile");
	newProjectile.AddMeshRenderer();
	newProjectile.AddMaterial();
	API_RigidBody rb = newProjectile.CreateRigidBodyBox((0, 0, 0), (0, 0, 0), (0.3f, 0.3f, 0.3f), false);
	rb.SetTrigger(true);
	newProjectile.AddParticleSystem(particleTest);
	newProjectile.AddScript("EnemyProjectile");
	newProjectile.SetActive(false);
	pull.push_back(newProjectile);*/
}

void TP_Cabin::DestroySphere()
{
	if (hasSpawnedSphere == true)
	{ 
		effectSphere.Destroy();
		effectSphere2.Destroy();
		hasSpawnedSphere = false;
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
			//Console::Log("PLAYER BEING DETECTED");
		}
		else
		{
			if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP)
			{
				canTp = true;
			}

			//Console::Log("PLAYER NOT BEING DETECTED");
			
			if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
			{
				timeHoldButton += Time::GetDeltaTime();
				sphereGrowingTime += Time::GetDeltaTime();
				//SpawnSphere();
			}
			else
			{
				timeHoldButton -= Time::GetDeltaTime();
				//sphereGrowingTime -= Time::GetDeltaTime();
				smoke.StopEmitting();
				destinationSmoke.StopEmitting();

				//DestroySphere();
			}

			if (canTp == true)
			{
				if (timeHoldButton > startParticles && timeHoldButton < endParticles )
				{
					smoke.Play();
					destinationSmoke.Play();
					Audio::Event("teleport_1");
				}
				else if (timeHoldButton > tpTime)
				{
					other.GetGameObject().GetTransform().SetPosition(destination.GetGlobalPosition());
					timeHoldButton = 0.0f;
					smoke.StopEmitting();
					destinationSmoke.StopEmitting();
					Audio::Event("teleport_2");

					canTp = false;
				}
			}
		}
	}
}
