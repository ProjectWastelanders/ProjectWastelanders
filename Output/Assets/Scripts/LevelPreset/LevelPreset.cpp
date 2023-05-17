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
	std::vector<std::string> levels = { "level1Selected" , "level2Selected", "level3Selected" , "level4Selected" };
	for (int i = 0; i < levels.size(); i++)
	{
		API_QuickSave::SetBool(levels.at(i), i == (lvlToSpawn - 1) ? true : false);
	}
}