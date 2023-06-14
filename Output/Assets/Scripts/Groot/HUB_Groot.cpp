#include "HUB_Groot.h"
HELLO_ENGINE_API_C HUB_Groot* CreateHUB_Groot(ScriptToInspectorInterface* script)
{
	HUB_Groot* classInstance = new HUB_Groot();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void HUB_Groot::Init()
{
	if (API_QuickSave::GetBool("level2_completed", false))
	{
		gameObject.SetActive(true);
	}
}
