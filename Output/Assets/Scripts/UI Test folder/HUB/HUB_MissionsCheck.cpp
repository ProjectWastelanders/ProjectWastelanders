#include "HUB_MissionsCheck.h"
HELLO_ENGINE_API_C HUB_MissionsCheck* CreateHUB_MissionsCheck(ScriptToInspectorInterface* script)
{
	HUB_MissionsCheck* classInstance = new HUB_MissionsCheck();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Checks parent", &classInstance->checksParent);
	script->AddInputBox("Principal Mission quicksave Name", &classInstance->missionsNames[0]);
	script->AddInputBox("Secondary Mission 1 quicksave Name", &classInstance->missionsNames[1]);
	script->AddInputBox("Secondary Mission 2 quicksave Name", &classInstance->missionsNames[2]);

	return classInstance;
}

void HUB_MissionsCheck::Start()
{
	checksParent.GetChildren(checks, 3);

	for (int i = 0; i < 3; ++i)
	{
		checks[i].SetActive(API_QuickSave::GetBool(missionsNames[i].c_str()));
	}
}
void HUB_MissionsCheck::Update()
{
	// Do every frame so it can be tested in debugging.
	for (int i = 0; i < 3; ++i)
	{
		checks[i].SetActive(API_QuickSave::GetBool(missionsNames[i].c_str()));
	}
}