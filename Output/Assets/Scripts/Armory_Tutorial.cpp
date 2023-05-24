#include "Armory_Tutorial.h"
HELLO_ENGINE_API_C Armory_Tutorial* CreateArmory_Tutorial(ScriptToInspectorInterface* script)
{
	Armory_Tutorial* classInstance = new Armory_Tutorial();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Armory Screen", &classInstance->armoryScreen);
	script->AddDragBoxGameObject("Tutorial 1", &classInstance->tutorial1);
	script->AddDragBoxGameObject("Tutorial 2", &classInstance->tutorial2);
	script->AddDragBoxGameObject("its Me", &classInstance->itsMe);
	return classInstance;
}

void Armory_Tutorial::Start()
{
	armoryScreen.SetActive(false);
	/*tutorial1_active = true;
	tutorial2_active = false;*/
	//activeTutorial = false;
	currentTutorial = 1;

	_script = (Armory_Tutorial*)itsMe.GetScript("Armory_Tutorial");
}
void Armory_Tutorial::Update()
{

	switch (currentTutorial)
	{
	case 1:
		Console::Log("JODER");
		PrintTutorial(tutorial1);
		break;
	case 2:
		Console::Log("TIO");
		PrintTutorial(tutorial2);
		break;
	default:
		break;
	}	
	/*if (tutorial1_active) 
	{
	 tutorial1.SetActive(true);
		 if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
		 {
			 tutorial1.SetActive(false);
			 tutorial2_active = true;
			 tutorial1_active = false;
			 Console::Log("JODER");
		 }
    }
	else if(tutorial2_active)
	{
		tutorial2.SetActive(true);
		Console::Log("TIO");
			if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
			{
				tutorial2.SetActive(false);
				tutorial2_active = false;
				armoryScreen.SetActive(true);
				_script->OnDisable();	
			}
	}*/
}

void Armory_Tutorial::PrintTutorial(API_GameObject& tutorial)
{
	tutorial.SetActive(true);
	if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN)
	{
		tutorial.SetActive(false);
		currentTutorial++;
		if (currentTutorial >= 2)
		{
			armoryScreen.SetActive(true);
			//_script->OnDisable();
		}		
	}
}
