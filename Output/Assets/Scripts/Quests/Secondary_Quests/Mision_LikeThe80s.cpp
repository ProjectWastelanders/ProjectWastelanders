#include "Mision_LikeThe80s.h"
#include "../../Player/PlayerStorage.h"

HELLO_ENGINE_API_C Mision_LikeThe80s* CreateMision_LikeThe80s(ScriptToInspectorInterface* script)
{
    Mision_LikeThe80s* classInstance = new Mision_LikeThe80s();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("PlayerStorage", &classInstance->playerStorageGO);
    script->AddCheckBox("Mision Completed", &classInstance->misionCompleted);

    return classInstance;
}

void Mision_LikeThe80s::Start()
{
    misionCompleted = API_QuickSave::GetBool("Mision_LikeThe80s");

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in Mision_LikeThe80s.");

}
void Mision_LikeThe80s::Update()
{

    if (playerStorage)
    {
        if (playerStorage->casette1Picked && playerStorage->casette2Picked && playerStorage->casette3Picked && misionCompleted == false)
        {

            playerStorage->skillPoints += 2;
            playerStorage->SaveData();

            misionCompleted = true;
        }
    }

}