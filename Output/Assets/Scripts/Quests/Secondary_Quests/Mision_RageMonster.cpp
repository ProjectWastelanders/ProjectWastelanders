#include "Mision_RageMonster.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C Mision_RageMonster* CreateMision_RageMonster(ScriptToInspectorInterface* script)
{
    Mision_RageMonster* classInstance = new Mision_RageMonster();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    return classInstance;
}

void Mision_RageMonster::Start()
{
    Game::FindGameObjectsWithTag("Box", &boxes[0], 16);

    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in GetDiviner Script.");
}

void Mision_RageMonster::Update()
{
    if (!misionCompleted)
    {
        bool completed = true;

        for (size_t i = 0; i < 16; i++)
        {
            if (boxes[i].IsActive()) completed = false;
        }

        misionCompleted = completed;
        if (playerStorage)
        {
            playerStorage->skillPoints += 2;
            playerStorage->SaveData();
        }
    }
}