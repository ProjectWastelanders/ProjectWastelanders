#include "SpinSword.h"
#include "ThanosAttacks.h"
HELLO_ENGINE_API_C SpinSword* CreateSpinSword(ScriptToInspectorInterface* script)
{
	SpinSword* classInstance = new SpinSword();
	script->AddDragBoxGameObject("Boss", &classInstance->boss);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void SpinSword::Start()
{
    Tattack = (ThanosAttacks*)boss.GetScript("ThanosAttacks");

}
void SpinSword::Update()
{
	spinCooldown += Time::GetDeltaTime();
	
	if (spinCooldown > 0.2f && spinState == SPINSTATE::STARTSPINNING) {
		spinState = SPINSTATE::SPINNING;
		Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosSpinAnimation);
		Tattack->thanosAnimationPlayer.SetLoop(true);
		Tattack->thanosAnimationPlayer.Play();
	}
	if (spinCooldown > 3.0f && spinState == SPINSTATE::SPINNING) {
		spinState = SPINSTATE::STOPSPINNING;
		Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosSpinAnimation);
		Tattack->thanosAnimationPlayer.SetLoop(false);
		Tattack->thanosAnimationPlayer.Play();
	}
	if (spinCooldown > 3.2f && spinState == SPINSTATE::STOPSPINNING) {
		spinCooldown = 0.0f;
		spinState = SPINSTATE::NOTSPINNING;
		Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosRunAnimation);
		Tattack->thanosAnimationPlayer.SetLoop(true);
		Tattack->thanosAnimationPlayer.Play();
	}

}
void SpinSword::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    std::string detectionTag = other.GetGameObject().GetTag();

	if (detectionTag == "Projectile") {
		
		switch (spinState)
		{

		case SPINSTATE::NOTSPINNING:
			spinCooldown = 0.0f;
			spinState = SPINSTATE::STARTSPINNING;
			Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosSpinAnimation);
			Tattack->thanosAnimationPlayer.SetLoop(false);
			Tattack->thanosAnimationPlayer.Play();

			break;

		case SPINSTATE::SPINNING:
			
			spinCooldown = 1.1f;
			
			break;

		case SPINSTATE::STOPSPINNING:
			spinCooldown = 1.1f;
			spinState = SPINSTATE::SPINNING;
			Tattack->thanosAnimationPlayer.ChangeAnimation(Tattack->thanosSpinAnimation);
			Tattack->thanosAnimationPlayer.SetLoop(true);
			Tattack->thanosAnimationPlayer.Play();

			break;
		default:
			break;
		}

		if (spinState == SPINSTATE::NOTSPINNING) {
		}
	}
   
}