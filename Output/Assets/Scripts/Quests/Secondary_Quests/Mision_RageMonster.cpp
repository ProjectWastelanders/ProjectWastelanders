#include "Mision_RageMonster.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C Mision_RageMonster* CreateMision_RageMonster(ScriptToInspectorInterface* script)
{
    Mision_RageMonster* classInstance = new Mision_RageMonster();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void Mision_RageMonster::Start()
{
    Game::FindGameObjectsWithTag("Box", boxes, 16);
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