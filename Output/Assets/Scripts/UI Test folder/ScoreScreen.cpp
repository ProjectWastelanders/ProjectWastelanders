#include "ScoreScreen.h"
HELLO_ENGINE_API_C ScoreScreen* CreateScoreScreen(ScriptToInspectorInterface* script)
{
	ScoreScreen* classInstance = new ScoreScreen();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Continue", &classInstance->continueButton);
	script->AddDragBoxUIButton("Return", &classInstance->returnButton);
	script->AddDragInt("Respawn in lvl:", &classInstance->lvlToSpawn);
	return classInstance;
}

void ScoreScreen::Start()
{
	hasToStart = true;
}

void ScoreScreen::Update()
{
	if (hasToStart == true) 
	{
		std::vector<std::string> levels = { "level1Selected" , "level2Selected", "level3Selected" , "level4Selected" };
		for (int i = 0; i < levels.size(); i++) 
		{
			if (API_QuickSave::GetBool(levels.at(i))) 
			{
				lvlToSpawn = i+1;
				break;
			}
		}

		if (lvlToSpawn == 1)
		{
			continueButton.GetGameObject().GetTransform().SetPosition(0.0f, -0.36f, -0.001f);
			returnButton.GetGameObject().SetActive(false);
		}

		hasToStart = false;
	}

	switch (lvlToSpawn)
	{
	case 1:
		if (continueButton.OnPress())
		{
			Scene::LoadScene("Level1.HScene");
		}
		break;
	case 2:
		if (continueButton.OnPress())
		{
			Scene::LoadScene("Level2.HScene");
		}
		break;
	case 3:
		if (continueButton.OnPress())
		{
			Scene::LoadScene("Level3.HScene");
		}
		break;
	case 4:
		if (continueButton.OnPress())
		{
			Scene::LoadScene("Level4.HScene");
		}
		break;
	default:

		break;
	}

	if (lvlToSpawn != 1)
	{
		if (returnButton.OnPress())
		{
			Scene::LoadScene("SpaceshipHUB_Scene.HScene");
		}
	}
	
}