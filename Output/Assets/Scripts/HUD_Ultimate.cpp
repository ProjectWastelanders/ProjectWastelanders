#include "HUD_Ultimate.h"
HELLO_ENGINE_API_C HUD_Ultimate* CreateHUD_Ultimate(ScriptToInspectorInterface* script)
{
    HUD_Ultimate* classInstance = new HUD_Ultimate();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxUIImage("Ultimate_HUD_On", &classInstance->hudActive);

    script->AddDragBoxGameObject("Player",&classInstance->playerUltimate_GO);
     
    return classInstance;
}

void HUD_Ultimate::Start()
{

    playerUltimate = (PlayerStats*)playerUltimate_GO.GetScript("PlayerStats");
    if (playerUltimate == nullptr) Console::Log("Missing PlayerStats on HUD_Ultimate Script.");

   //HudActive.SetActive(true);
}
void HUD_Ultimate::Update()
{
    HUD_Ultimate_Active(playerUltimate->ultPercentage / 100);
}

void HUD_Ultimate::HUD_Ultimate_Active(float IsActive)
{
    hudActive.FillImage(IsActive);
}
