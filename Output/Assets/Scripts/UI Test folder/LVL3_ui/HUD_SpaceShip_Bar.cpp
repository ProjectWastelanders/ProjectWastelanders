#include "HUD_SpaceShip_Bar.h"
HELLO_ENGINE_API_C HUD_SpaceShip_Bar* CreateHUD_SpaceShip_Bar(ScriptToInspectorInterface* script)
{
	HUD_SpaceShip_Bar* classInstance = new HUD_SpaceShip_Bar();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void HUD_SpaceShip_Bar::Start()
{

}
void HUD_SpaceShip_Bar::Update()
{

}