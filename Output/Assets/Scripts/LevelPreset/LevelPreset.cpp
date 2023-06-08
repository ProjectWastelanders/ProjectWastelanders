#include "LevelPreset.h"
HELLO_ENGINE_API_C LevelPreset* CreateLevelPreset(ScriptToInspectorInterface* script)
{
	LevelPreset* classInstance = new LevelPreset();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragInt("Respawn in lvl:", &classInstance->lvlToSpawn);
	return classInstance;
}

void LevelPreset::Start()
{
	API_QuickSave::SetInt("currentSelectedLevel", lvlToSpawn-1);
}