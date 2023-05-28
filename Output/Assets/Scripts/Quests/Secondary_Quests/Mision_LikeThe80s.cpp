#include "Mision_LikeThe80s.h"
#include "../../Player/PlayerStorage.h"

HELLO_ENGINE_API_C Mision_LikeThe80s* CreateMision_LikeThe80s(ScriptToInspectorInterface* script)
{
    Mision_LikeThe80s* classInstance = new Mision_LikeThe80s();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("PlayerStorage", &classInstance->playerStorageGO);

    return classInstance;
}

void Mision_LikeThe80s::Start()
{

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in Mision_LikeThe80s.");

}
void Mision_LikeThe80s::Update()
{

    if (misionCompleted)
    {
        Console::Log("MISSION 3 COMPLETED CONGRATULATIONS!");
    }
    else if (playerStorage)
    {
        if (playerStorage->casette1Picked && playerStorage->casette2Picked && playerStorage->casette3Picked)
        {

            playerStorage->skillPoints += 2;
            playerStorage->SaveData();

            misionCompleted = true;
        }
    }

}