#include "HUD_Ultimate.h"
HELLO_ENGINE_API_C HUD_Ultimate* CreateHUD_Ultimate(ScriptToInspectorInterface* script)
{
    HUD_Ultimate* classInstance = new HUD_Ultimate();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    //script->AddDragBoxGameObject("Disable_Ultimate_HUD", &classInstance->HudActive);
    //script->AddDragBoxGameObject("Active_Ultimate_HUD", &classInstance->HudDisactive);
    //
    return classInstance;
}

void HUD_Ultimate::Start()
{
   //HudDisactive.SetActive(false);
   //HudActive.SetActive(true);
}
void HUD_Ultimate::Update()
{
    //HUD_Ultimate_Active(UltimateAvaliable);
}

void HUD_Ultimate::HUD_Ultimate_Active(bool IsActive)
{
}
