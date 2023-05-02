#include "Mision_RageMonster.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C Mision_RageMonster* CreateMision_RageMonster(ScriptToInspectorInterface* script)
{
    Mision_RageMonster* classInstance = new Mision_RageMonster();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Box 0", &classInstance->boxes[0]);
    script->AddDragBoxGameObject("Box 1", &classInstance->boxes[1]);
    script->AddDragBoxGameObject("Box 2", &classInstance->boxes[2]);
    script->AddDragBoxGameObject("Box 3", &classInstance->boxes[3]);
    script->AddDragBoxGameObject("Box 4", &classInstance->boxes[4]);
    script->AddDragBoxGameObject("Box 5", &classInstance->boxes[5]);
    script->AddDragBoxGameObject("Box 6", &classInstance->boxes[6]);
    script->AddDragBoxGameObject("Box 7", &classInstance->boxes[7]);
    script->AddDragBoxGameObject("Box 8", &classInstance->boxes[8]);
    script->AddDragBoxGameObject("Box 9", &classInstance->boxes[9]);
    script->AddDragBoxGameObject("Box 10", &classInstance->boxes[10]);
    script->AddDragBoxGameObject("Box 11", &classInstance->boxes[11]);
    script->AddDragBoxGameObject("Box 12", &classInstance->boxes[12]);
    script->AddDragBoxGameObject("Box 13", &classInstance->boxes[13]);
    script->AddDragBoxGameObject("Box 14", &classInstance->boxes[14]);
    script->AddDragBoxGameObject("Box 15", &classInstance->boxes[15]);
    return classInstance;
}

void Mision_RageMonster::Start()
{

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