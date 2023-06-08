#include "Armory_Tutorial.h"
HELLO_ENGINE_API_C Armory_Tutorial* CreateArmory_Tutorial(ScriptToInspectorInterface* script)
{
	Armory_Tutorial* classInstance = new Armory_Tutorial();

	script->AddDragBoxGameObject("Tutorial 1", &classInstance->tutorials[0]);
	script->AddDragBoxGameObject("Tutorial 2", &classInstance->tutorials[1]);

	script->AddDragBoxUIInput("Armory Main Panel", &classInstance->armoryScreen);

	return classInstance;
}

void Armory_Tutorial::Start()
{
	if (!API_QuickSave::GetBool("Armory_Tutorial", false))
	{
		tutorial = new UITutorial(tutorials, 2);
		tutorials[0].SetActive(true);
		tutorials[1].SetActive(false);
		Console::Log("Tut!");
	}
	else
	{
		tutorial = nullptr;
		tutorials[0].SetActive(false);
		tutorials[1].SetActive(false);
	}
}

void Armory_Tutorial::Update()
{
	if (tutorial != nullptr)
	{
		armoryScreen.SetEnable(false);
		if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
		{
			if (!tutorial->NextScreen())
			{
				tutorial->Unable();
				delete tutorial;
				tutorial = nullptr;
				armoryScreen.SetEnable(true);
				API_QuickSave::SetBool("Armory_Tutorial", true);
			}
		}
		return;
	}
}
