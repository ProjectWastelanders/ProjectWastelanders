#include "ScoreScreen.h"
HELLO_ENGINE_API_C ScoreScreen* CreateScoreScreen(ScriptToInspectorInterface* script)
{
	ScoreScreen* classInstance = new ScoreScreen();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Continue", &classInstance->continueButton);
	script->AddDragInt("Screen type (0:LEVEL2,1:WIN,2:LOSE)", &classInstance->screenType);
	return classInstance;
}

void ScoreScreen::Start()
{

}
void ScoreScreen::Update()
{
	if (screenType == 0)
	{
		if (continueButton.OnPress())
		{
			Scene::LoadScene("LVL2_Blockout.HScene");
		}
	}
	else
	{
		if (continueButton.OnPress())
		{
			Scene::LoadScene("SpaceshipHUB_Scene.HScene");
		}
	}
}