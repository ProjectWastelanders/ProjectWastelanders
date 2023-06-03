#include "Levels_Information_Save.h"
HELLO_ENGINE_API_C Levels_Information_Save* CreateLevels_Information_Save(ScriptToInspectorInterface* script)
{
	Levels_Information_Save* classInstance = new Levels_Information_Save();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIText("CaseteInfo_Text", &classInstance->casete_text);
	script->AddDragBoxUIText("BluePrintsInfo_Text", &classInstance->bluePrint_text);
	script->AddDragBoxUIText("GoldPrintInfo_Text", &classInstance->goldPrint_text);
	script->AddDragInt("levels", &classInstance->levels);
	return classInstance;
}

void Levels_Information_Save::Start()
{
	if (levels < 0 || levels > 3)
		return;

	std::string levelsString[4] = { "level1", "level2", "level3", "level4" };

	bool tempBool[3] = { false, false, false };

	int counter = 0;

	// Get Cassetes numbers
	tempBool[0] = API_QuickSave::GetBool(levelsString[levels] + "_casette1");
	tempBool[1] = API_QuickSave::GetBool(levelsString[levels] + "_casette2");
	tempBool[2] = API_QuickSave::GetBool(levelsString[levels] + "_casette3");

	for (int i = 0; i < 3; i++)
	{
		if (tempBool[i])
			counter++;
	}

	casete_text.SetText(std::to_string(counter).c_str());

	// Get GoldPrint numbers
	tempBool[0] = API_QuickSave::GetBool(levelsString[levels] + "_chest2");
	tempBool[1] = API_QuickSave::GetBool(levelsString[levels] + "_chest3");
	tempBool[2] = API_QuickSave::GetBool(levelsString[levels] + "_chest4");
	
	counter = 0;

	for (int i = 0; i < 3; i++)
	{
		if (tempBool[i])
			counter++;
	}

	goldPrint_text.SetText(std::to_string(counter).c_str());

	// Get BluePrint numbers
	tempBool[0] = API_QuickSave::GetBool(levelsString[levels] + "_chest1");

	counter = tempBool[0] ? 1 : 0;

	bluePrint_text.SetText(std::to_string(counter).c_str());
}
void Levels_Information_Save::Update()
{

}