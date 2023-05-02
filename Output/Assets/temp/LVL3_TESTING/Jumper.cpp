#include "Jumper.h"

HELLO_ENGINE_API_C Jumper* CreateJumper(ScriptToInspectorInterface* script)
{
	Jumper* classInstance = new Jumper();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Player", classInstance->player);
	script->AddDragBoxGameObject("Connected_Jumper", classInstance->otherJumper);
	script->AddDragFloat("Jump Duration", &classInstance->duration);

	return classInstance;
}

void Jumper::Start()
{
	validScript = true;

	if (player == nullptr) {
		Console::Log("< Player > not dragged in Jumper script inspector.");
		validScript = false;
	}
	if (otherJumper == nullptr) {
		Console::Log("< Connected_Jumper > not dragged in Jumper script inspector.");
		validScript = false;
	}
}

void Jumper::Update()
{
	if (!validScript) return;

	//IterateJump
	if (isJumping) {
		API_Vector3 pos;
		pos.x = Lerp(initialPos.x, finalPos.x, timeJumping / duration);
		pos.y = Lerp(initialPos.y, finalPos.y, timeJumping / duration);
		pos.z = Lerp(initialPos.z, finalPos.z, timeJumping / duration);

		player->GetTransform().SetPosition(pos);
		timeJumping += Time::GetDeltaTime();

		if (timeJumping > duration) isJumping = false;
	}
}

float Jumper::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void Jumper::SetupJump()
{
	timeJumping = 0.0f;
	initialPos = player->GetTransform().GetGlobalPosition();
	finalPos = otherJumper->GetTransform().GetGlobalPosition();
}