#include "EnemyManager.h"
HELLO_ENGINE_API_C EnemyManager* CreateEnemyManager(ScriptToInspectorInterface* script)
{
    EnemyManager* classInstance = new EnemyManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragInt("Enemies Amount", &classInstance->enemiesAmount);
    script->AddDragBoxGameObject("Enemy 1", &classInstance->enemies[0]);
    script->AddDragBoxGameObject("Enemy 2", &classInstance->enemies[1]);
    script->AddDragBoxGameObject("Enemy 3", &classInstance->enemies[2]);
    script->AddDragBoxGameObject("Enemy 4", &classInstance->enemies[3]);
    script->AddDragBoxGameObject("Enemy 5", &classInstance->enemies[4]);
    script->AddDragBoxGameObject("Enemy 6", &classInstance->enemies[5]);
    script->AddDragBoxGameObject("Enemy 7", &classInstance->enemies[6]);
    script->AddDragBoxGameObject("Enemy 8", &classInstance->enemies[7]);
    script->AddDragBoxGameObject("Enemy 9", &classInstance->enemies[8]);
    script->AddDragBoxGameObject("Enemy 10", &classInstance->enemies[9]);
    script->AddDragBoxGameObject("Enemy 11", &classInstance->enemies[10]);
    script->AddDragBoxGameObject("Enemy 12", &classInstance->enemies[11]);
    script->AddDragBoxGameObject("Enemy 13", &classInstance->enemies[12]);
    script->AddDragBoxGameObject("Enemy 14", &classInstance->enemies[13]);
    script->AddDragBoxGameObject("Enemy 15", &classInstance->enemies[14]);
    script->AddDragBoxGameObject("Enemy 16", &classInstance->enemies[15]);
    script->AddDragBoxGameObject("Enemy 17", &classInstance->enemies[16]);
    script->AddDragBoxGameObject("Enemy 18", &classInstance->enemies[17]);
    script->AddDragBoxGameObject("Enemy 19", &classInstance->enemies[18]);
    script->AddDragBoxGameObject("Enemy 20", &classInstance->enemies[19]);
    script->AddDragBoxGameObject("Enemy 21", &classInstance->enemies[20]);
    script->AddDragBoxGameObject("Enemy 22", &classInstance->enemies[21]);
    script->AddDragBoxGameObject("Enemy 23", &classInstance->enemies[22]);
    script->AddDragBoxGameObject("Enemy 24", &classInstance->enemies[23]);
    script->AddDragBoxGameObject("Enemy 25", &classInstance->enemies[24]);
    script->AddDragBoxGameObject("Enemy 26", &classInstance->enemies[25]);
    script->AddDragBoxGameObject("Enemy 27", &classInstance->enemies[26]);
    script->AddDragBoxGameObject("Enemy 28", &classInstance->enemies[27]);
    script->AddDragBoxGameObject("Enemy 29", &classInstance->enemies[28]);
    script->AddDragBoxGameObject("Enemy 30", &classInstance->enemies[29]);
    script->AddDragBoxGameObject("Enemy 31", &classInstance->enemies[30]);
    script->AddDragBoxGameObject("Enemy 32", &classInstance->enemies[31]);
    script->AddDragBoxGameObject("Enemy 33", &classInstance->enemies[32]);
    script->AddDragBoxGameObject("Enemy 34", &classInstance->enemies[33]);
    script->AddDragBoxGameObject("Enemy 35", &classInstance->enemies[34]);
    script->AddDragBoxGameObject("Enemy 36", &classInstance->enemies[35]);
    script->AddDragBoxGameObject("Enemy 37", &classInstance->enemies[36]);
    script->AddDragBoxGameObject("Enemy 38", &classInstance->enemies[37]);
    script->AddDragBoxGameObject("Enemy 39", &classInstance->enemies[38]);
    script->AddDragBoxGameObject("Enemy 40", &classInstance->enemies[39]);
    script->AddDragBoxGameObject("Enemy 41", &classInstance->enemies[40]);
    script->AddDragBoxGameObject("Enemy 42", &classInstance->enemies[41]);
    script->AddDragBoxGameObject("Enemy 43", &classInstance->enemies[42]);
    script->AddDragBoxGameObject("Enemy 44", &classInstance->enemies[43]);
    script->AddDragBoxGameObject("Enemy 45", &classInstance->enemies[44]);
    script->AddDragBoxGameObject("Enemy 46", &classInstance->enemies[45]);
    script->AddDragBoxGameObject("Enemy 47", &classInstance->enemies[46]);
    script->AddDragBoxGameObject("Enemy 48", &classInstance->enemies[47]);
    script->AddDragBoxGameObject("Enemy 49", &classInstance->enemies[48]);
    script->AddDragBoxGameObject("Enemy 50", &classInstance->enemies[49]);
    script->AddDragBoxGameObject("Enemy 51", &classInstance->enemies[50]);
    script->AddDragBoxGameObject("Enemy 52", &classInstance->enemies[51]);
    script->AddDragBoxGameObject("Enemy 53", &classInstance->enemies[52]);
    script->AddDragBoxGameObject("Enemy 54", &classInstance->enemies[53]);
    script->AddDragBoxGameObject("Enemy 55", &classInstance->enemies[54]);
    script->AddDragBoxGameObject("Enemy 56", &classInstance->enemies[55]);
    script->AddDragBoxGameObject("Enemy 57", &classInstance->enemies[56]);
    script->AddDragBoxGameObject("Enemy 58", &classInstance->enemies[57]);
    script->AddDragBoxGameObject("Enemy 59", &classInstance->enemies[58]);
    script->AddDragBoxGameObject("Enemy 60", &classInstance->enemies[59]);
    script->AddDragBoxGameObject("Enemy 61", &classInstance->enemies[50]);
    script->AddDragBoxGameObject("Enemy 62", &classInstance->enemies[51]);
    script->AddDragBoxGameObject("Enemy 63", &classInstance->enemies[52]);
    script->AddDragBoxGameObject("Enemy 64", &classInstance->enemies[53]);
    script->AddDragBoxGameObject("Enemy 65", &classInstance->enemies[54]);
    script->AddDragBoxGameObject("Enemy 66", &classInstance->enemies[55]);
    script->AddDragBoxGameObject("Enemy 67", &classInstance->enemies[56]);
    script->AddDragBoxGameObject("Enemy 68", &classInstance->enemies[57]);
    script->AddDragBoxGameObject("Enemy 69", &classInstance->enemies[58]);
    return classInstance;
}

void EnemyManager::Start()
{
    
}

void EnemyManager::Update()
{

}

void EnemyManager::SaveEnemiesState(int levelIndex)
{
    std::string aux = "lvl" + std::to_string(levelIndex) + "_enemy";
    for (int i = 0; i < enemiesAmount; i++)
    {
        std::string name = aux + std::to_string(i);
        API_QuickSave::SetBool(name.c_str(), enemies[i].IsActive());
    }
}

void EnemyManager::LoadEnemiesState(int levelIndex)
{
    std::string aux = "lvl" + std::to_string(levelIndex) + "_enemy";
    for (int i = 0; i < enemiesAmount; i++)
    {
        std::string name = aux + std::to_string(i);
        if (!API_QuickSave::GetBool(name.c_str(), true)) enemies[i].SetActive(false);
    }
}