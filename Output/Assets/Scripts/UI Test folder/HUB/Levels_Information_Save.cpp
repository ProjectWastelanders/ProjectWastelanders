#include "Levels_Information_Save.h"
HELLO_ENGINE_API_C Levels_Information_Save* CreateLevels_Information_Save(ScriptToInspectorInterface* script)
{
	Levels_Information_Save* classInstance = new Levels_Information_Save();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIText("CaseteInfo_Text", &classInstance->casete_text);
	script->AddDragBoxUIText("BluePrintsInfo_Text", &classInstance->bluePrint_text);
	script->AddDragBoxUIText("GoldPrintInfo_Text", &classInstance->goldPrint_text);
	script->AddDragInt("levels", &classInstance->levels);
	script->AddDragInt("casetes", &classInstance->casetes);
	return classInstance;
}

void Levels_Information_Save::Start()
{
	std::string temp[4] = { "level1", "level2", "level3", "level4" };
	bool casetes1 = false, casetes2 = false, casetes3 = false;
	if (levels >= 0 && levels <= 3)
	{
		casetes1 = API_QuickSave::GetBool(temp[levels] + "_casettes1");
		casetes2 = API_QuickSave::GetBool(temp[levels] + "_casettes2");
		casetes3 = API_QuickSave::GetBool(temp[levels] + "_casettes3");
	}

	if (casetes1 == true)
	{
		casetes++;
	}
	if (casetes2 == true)
	{
		casetes++;
	}
	if (casetes3 == true)
	{
		casetes++;
	}

	casete_text.SetText(std::to_string(casetes).c_str());

	if (levels >= 0 && levels <= 3)
	{
		casetes1 = API_QuickSave::GetBool(temp[levels] + "_chest2");
		casetes2 = API_QuickSave::GetBool(temp[levels] + "_chest3");
		casetes3 = API_QuickSave::GetBool(temp[levels] + "_chest4");
	}
	casetes = 0;
	if (casetes1 == true)
	{
		casetes++;
	}
	if (casetes2 == true)
	{
		casetes++;
	}
	if (casetes3 == true)
	{
		casetes++;
	}

	goldPrint_text.SetText(std::to_string(casetes).c_str());
	
	if (levels >= 0 && levels <= 3)
	{
		casetes1 = API_QuickSave::GetBool(temp[levels] + "_chest1");
	}
	if (casetes1 == true)
	{
		casetes = 1;
	}
	else
	{
		casetes = 0;
	}
	goldPrint_text.SetText(std::to_string(casetes).c_str());

}
void Levels_Information_Save::Update()
{

}