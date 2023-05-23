#include "GrootAnimationScript.h"
HELLO_ENGINE_API_C GrootAnimationScript* CreateGrootAnimationScript(ScriptToInspectorInterface* script)
{
	GrootAnimationScript* classInstance = new GrootAnimationScript();
	script->AddDragBoxAnimationPlayer("AnimationPlayer", &classInstance->grootAnimationPlayer);
	script->AddDragBoxAnimationResource("Groot Dance", &classInstance->grootDance);
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void GrootAnimationScript::Start()
{
	grootAnimationPlayer.ChangeAnimation(grootDance);
	grootAnimationPlayer.SetLoop(true);
	grootAnimationPlayer.Play();
}
void GrootAnimationScript::Update()
{

}