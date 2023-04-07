#include "SmokeExtractorTrap.h"
#include "../Player/PlayerStats.h"

HELLO_ENGINE_API_C SmokeExtractorTrap* CreateSmokeExtractorTrap(ScriptToInspectorInterface* script)
{
	SmokeExtractorTrap* classInstance = new SmokeExtractorTrap();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxParticleSystem("Smoke Particle", &classInstance->smoke);
	script->AddDragBoxParticleSystem("Fire Particle", &classInstance->fire);
	//Cuanto tarda a empezar a escupir fuego
	script->AddDragFloat("Delay on Hit", &classInstance->delay);
	script->AddDragFloat("Seconds emits fire", &classInstance->fireSeconds);

	return classInstance;
}

void SmokeExtractorTrap::Start()
{
	smoke.Play();

	currentDelay = delay;

	fireTimer = 0.0f;
}
void SmokeExtractorTrap::Update()
{
	currentDelay -= Time::GetDeltaTime();

	if (currentDelay <= 0.0f && fireTimer < fireSeconds)
	{
		fireTimer += Time::GetDeltaTime();

		smoke.StopEmitting();
		fire.Play();

		throwFire = true;

	}
	else
	{

		if (fireTimer > fireSeconds)
		{
			currentDelay = delay;
		}

		fireTimer = 0.0f;

		fire.StopEmitting();
		smoke.Play();

		throwFire = false;

	}
}

void SmokeExtractorTrap::OnCollisionEnter(API_RigidBody other)
{

	std::string detectionName = other.GetGameObject().GetName();

	if (detectionName == "Player")
	{
		if (throwFire)
		{
			PlayerStats* playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");

			playerStats->TakeDamage(10.0f, 0);
		}
	}
}
