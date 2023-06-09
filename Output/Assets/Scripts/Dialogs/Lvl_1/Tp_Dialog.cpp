#include "Tp_Dialog.h"
#include "../../Player/PlayerStats.h"

HELLO_ENGINE_API_C Tp_Dialog* CreateTp_Dialog(ScriptToInspectorInterface* script)
{
	Tp_Dialog* classInstance = new Tp_Dialog();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIImage("Dialog 1", &classInstance->Dialog_1);

	script->AddDragFloat("Dialog timer", &classInstance->timer);

	script->AddDragFloat("Dialog Pos X", &classInstance->finalPosDialog.x);
	script->AddDragFloat("Dialog Pos Y", &classInstance->finalPosDialog.y);
	script->AddDragFloat("Dialog Pos Z", &classInstance->finalPosDialog.z);
	return classInstance;
}

void Tp_Dialog::Start()
{
	Game::FindGameObjectsWithTag("Player", &playerGO, 1);

	playerStats = (PlayerStats*)playerGO.GetScript("PlayerStats");
	if (playerStats == nullptr) Console::Log("Player is missing in TP CABIN Script.");

	initalPosDialog = { 0, -1.500, 0 };
	movingPosDialog = { 0, -1.500, 0 };

	activeAudio = true;

	Dialog_1.GetGameObject().GetTransform().SetPosition(initalPosDialog);
	Dialog_1.GetGameObject().SetActive(false);
}
void Tp_Dialog::Update()
{
	if (playerStats != nullptr) {
		if (playerStats->showTpDialog)
		{
			PrintDialog(Dialog_1);
			if (activeAudio) {
				Audio::Event("rocket_surprised");
				activeAudio = false;
			}
		}
	}
}

void Tp_Dialog::PrintDialog(API_UIImage& Dialog)
{
	Dialog.GetGameObject().SetActive(true);
	if (_timer >= timer) {
		if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPosDialog.y)
		{
			movingPosDialog.y -= 1 * Time::GetDeltaTime();
		}
		else {
			playerStats->showedTpDialog = true;
			playerStats->showTpDialog = false;
		}
	}
	else {
		if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y < finalPosDialog.y)
		{
			movingPosDialog.y += 1 * Time::GetDeltaTime();
		}
		else
		{
			_timer += Time::GetDeltaTime();
		}
	}

	Dialog.GetGameObject().GetTransform().SetPosition(movingPosDialog);
}