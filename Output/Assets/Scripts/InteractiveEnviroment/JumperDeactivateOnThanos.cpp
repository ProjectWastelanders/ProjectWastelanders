#include "JumperDeactivateOnThanos.h"
#include "Jumper.h"
#include "../Dialogs/Lvl4/ThanosCinematic.h"
#include "../Thanos/ThanosLoop.h"
HELLO_ENGINE_API_C JumperDeactivateOnThanos* CreateJumperDeactivateOnThanos(ScriptToInspectorInterface* script)
{
	JumperDeactivateOnThanos* classInstance = new JumperDeactivateOnThanos();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("ThanosCinematicGO", &classInstance->thanosCinematicGO);
	script->AddDragBoxGameObject("JumperGO", &classInstance->jumper);
	script->AddDragBoxGameObject("ThanosLoopGO", &classInstance->thanosLoopGO);
	script->AddDragBoxGameObject("Light", &classInstance->LightGO);
	return classInstance;
}

void JumperDeactivateOnThanos::Start()
{
	jumperRef = (Jumper*)jumper.GetScript("Jumper");
	if (jumperRef == nullptr) Console::Log("Jumper missing in JumperDeactivateOnThanos Script.");

	thanosCinematicRef = (ThanosCinematic*)thanosCinematicGO.GetScript("ThanosCinematic");
	if (thanosCinematicRef == nullptr) Console::Log("ThanosCinematicGO missing in JumperDeactivateOnThanos Script.");

	thanosLoopRef = (ThanosLoop*)thanosLoopGO.GetScript("ThanosLoop");
	if (thanosLoopRef == nullptr) Console::Log("thanosLoopGO missing in JumperDeactivateOnThanos Script.");

}
void JumperDeactivateOnThanos::Update()
{
	
	if(thanosLoopRef->hp <= 0)
	{
		jumperRef->Inactive = false;
		LightGO.SetActive(false);
	}
	else if (thanosCinematicRef->activeCinematic)
	{
		jumperRef->Inactive = true;
		LightGO.SetActive(true);
	}
} 