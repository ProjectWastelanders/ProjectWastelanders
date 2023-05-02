#include "Jumper.h"
#include "../../Scripts/Player/PlayerMove.h"

HELLO_ENGINE_API_C Jumper* CreateJumper(ScriptToInspectorInterface* script)
{
	Jumper* classInstance = new Jumper();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxTransform("Connected_Jumper", &classInstance->otherJumper);
	script->AddDragFloat("Jump Duration", &classInstance->duration);

	return classInstance;
}

void Jumper::Start()
{
	Game::FindGameObjectsWithTag("Player", &player, 1);
	playerMoveScript = (PlayerMove*)player.GetScript("PlayerMove");
	if (playerMoveScript == nullptr) 
	{
		Console::Log("(<Jumper> error) Player GO is missing <PlayerMove> script.");
	}
}

void Jumper::Update()
{
	//IterateJump
	if (isJumping) {
		API_Vector3 pos;
		pos.x = Lerp(initialPos.x, finalPos.x, timeJumping / duration);
		pos.y = Lerp(initialPos.y, finalPos.y, timeJumping / duration);
		pos.z = Lerp(initialPos.z, finalPos.z, timeJumping / duration);

		player.GetTransform().SetPosition(pos);
		timeJumping += Time::GetDeltaTime();

		//Finished Jumping
		if (timeJumping > duration) {
			isJumping = false;
			
			//Stun
			if (playerMoveScript != nullptr)
				playerMoveScript->openingChest = false;
		}
	}
}

void Jumper::OnCollisionEnter(API_RigidBody other)
{
	std::string tag = other.GetGameObject().GetTag();
	if (tag == "Player") {
		SetupJump();
	}
}

float Jumper::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void Jumper::SetupJump()
{
	if (isJumping) return;

	//Stun & Animation
	if (playerMoveScript != nullptr) {
		playerMoveScript->openingChest = true;
		playerMoveScript->PlayJumperAnim();
	}

	initialPos = player.GetTransform().GetGlobalPosition();
	finalPos = otherJumper.GetGlobalPosition();
	timeJumping = 0.0f;
	isJumping = true;
}