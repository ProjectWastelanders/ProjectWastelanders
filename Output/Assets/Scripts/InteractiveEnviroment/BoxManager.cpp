#include "BoxManager.h"
HELLO_ENGINE_API_C BoxManager* CreateBoxManager(ScriptToInspectorInterface* script)
{
    BoxManager* classInstance = new BoxManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Boxes Amount", &classInstance->boxesAmount);
    script->AddDragBoxGameObject("Box 1", &classInstance->boxes[0]);
    script->AddDragBoxGameObject("Box 2", &classInstance->boxes[1]);
    script->AddDragBoxGameObject("Box 3", &classInstance->boxes[2]);
    script->AddDragBoxGameObject("Box 4", &classInstance->boxes[3]);
    script->AddDragBoxGameObject("Box 5", &classInstance->boxes[4]);
    script->AddDragBoxGameObject("Box 6", &classInstance->boxes[5]);
    script->AddDragBoxGameObject("Box 7", &classInstance->boxes[6]);
    script->AddDragBoxGameObject("Box 8", &classInstance->boxes[7]);
    script->AddDragBoxGameObject("Box 9", &classInstance->boxes[8]);
    script->AddDragBoxGameObject("Box 10", &classInstance->boxes[9]);
    script->AddDragBoxGameObject("Box 11", &classInstance->boxes[10]);
    script->AddDragBoxGameObject("Box 12", &classInstance->boxes[11]);
    script->AddDragBoxGameObject("Box 13", &classInstance->boxes[12]);
    script->AddDragBoxGameObject("Box 14", &classInstance->boxes[13]);
    script->AddDragBoxGameObject("Box 15", &classInstance->boxes[14]);
    script->AddDragBoxGameObject("Box 16", &classInstance->boxes[15]);
    script->AddDragBoxGameObject("Box 17", &classInstance->boxes[16]);
    script->AddDragBoxGameObject("Box 18", &classInstance->boxes[17]);
    script->AddDragBoxGameObject("Box 19", &classInstance->boxes[18]);
    script->AddDragBoxGameObject("Box 20", &classInstance->boxes[19]);
    script->AddDragBoxGameObject("Box 21", &classInstance->boxes[20]);
    script->AddDragBoxGameObject("Box 22", &classInstance->boxes[21]);
    script->AddDragBoxGameObject("Box 23", &classInstance->boxes[22]);
    script->AddDragBoxGameObject("Box 24", &classInstance->boxes[23]);
    script->AddDragBoxGameObject("Box 25", &classInstance->boxes[24]);
    script->AddDragBoxGameObject("Box 26", &classInstance->boxes[25]);
    script->AddDragBoxGameObject("Box 27", &classInstance->boxes[26]);
    script->AddDragBoxGameObject("Box 28", &classInstance->boxes[27]);
    script->AddDragBoxGameObject("Box 29", &classInstance->boxes[28]);
    script->AddDragBoxGameObject("Box 30", &classInstance->boxes[29]);
    return classInstance;
}

void BoxManager::Start()
{
    
}

void BoxManager::Update()
{

}

void BoxManager::SaveBoxesState(int levelIndex)
{
    std::string aux = "lvl" + std::to_string(levelIndex) + "box";
    for (int i = 0; i < boxesAmount; i++)
    {
        std::string name = aux + std::to_string(i);
        API_QuickSave::SetBool(name.c_str(), boxes[i].IsAlive());
    }
}

void BoxManager::LoadBoxesState(int levelIndex)
{
    std::string aux = "lvl" + std::to_string(levelIndex) + "box";
    for (int i = 0; i < boxesAmount; i++)
    {
        std::string name = aux + std::to_string(i);
        if (!API_QuickSave::GetBool(name.c_str(), true)) boxes[i].SetActive(false);
    }
}