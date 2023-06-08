#include "Mision_SomethingPersonal.h"
#include "../../Player/PlayerStorage.h"

HELLO_ENGINE_API_C Mision_SomethingPersonal* CreateMision_SomethingPersonal(ScriptToInspectorInterface* script)
{
	Mision_SomethingPersonal* classInstance = new Mision_SomethingPersonal();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("hiddentank", &classInstance->hiddenTank);
	script->AddDragBoxGameObject("Player Storage", &classInstance->playerStorageGO);

	return classInstance;
}

void Mision_SomethingPersonal::Start()
{
	hiddenTank = Game::FindGameObject("HiddenTank");

	misionCompleted = API_QuickSave::GetBool("mision_Personal");

	playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
	if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");
}

void Mision_SomethingPersonal::Update()
{
	if (misionCompleted == false)
	{
		if (hiddenTank.IsActive() == false)
		{

			misionCompleted = true;

			playerStorage->skillPoints += 2;
			playerStorage->SaveData();

		}

	}
}