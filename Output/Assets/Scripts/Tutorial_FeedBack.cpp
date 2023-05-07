#include "Tutorial_FeedBack.h"
HELLO_ENGINE_API_C Tutorial_FeedBack* CreateTutorial_FeedBack(ScriptToInspectorInterface* script)
{
	Tutorial_FeedBack* classInstance = new Tutorial_FeedBack();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	//script->AddDragBoxUIImage("Tutorial_Img", &classInstance->Tutorial_Img);
	return classInstance;
}

void Tutorial_FeedBack::Start()
{
}
void Tutorial_FeedBack::Update()
{
}