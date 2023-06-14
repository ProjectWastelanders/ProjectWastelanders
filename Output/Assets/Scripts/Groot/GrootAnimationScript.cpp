#include "GrootAnimationScript.h"
HELLO_ENGINE_API_C GrootAnimationScript* CreateGrootAnimationScript(ScriptToInspectorInterface* script)
{
	GrootAnimationScript* classInstance = new GrootAnimationScript();
	script->AddDragBoxAnimationPlayer("AnimationPlayer", &classInstance->grootAnimationPlayer);
	script->AddDragBoxAnimationResource("Groot Escape", &classInstance->grootEscape);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void GrootAnimationScript::Start()
{
	grootAnimationPlayer.ChangeAnimation(grootEscape);
	grootAnimationPlayer.SetLoop(true);
	grootAnimationPlayer.Play();
}
void GrootAnimationScript::Update()
{

}