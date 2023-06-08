#include "FadeToBlack.h"
HELLO_ENGINE_API_C FadeToBlack* CreateFadeToBlack(ScriptToInspectorInterface* script)
{
	FadeToBlack* classInstance = new FadeToBlack();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxUIImage("Black Image", &classInstance->blackImage);
	script->AddCheckBox("Fade to black", &classInstance->fadeToBlack);
	script->AddCheckBox("Black to fade", &classInstance->blackToFade);

	return classInstance;
}

void FadeToBlack::Init()
{

	gameObject.SetActive(true);

}

void FadeToBlack::Start()
{

	blackImage.SetOpacity(0.0f);

}


void FadeToBlack::Update()
{

	fadeTimer += Time::GetDeltaTime();
	 
	if (fadeToBlack)
	{
		FadeIN();
	}
	else if (blackToFade)
	{
		FadeOUT();
	}
	else
	{
		fadeTimer = 0.0f;
		faded = false;
	}
	
}

void FadeToBlack::SetOpacity(float opacity)
{
	blackImage.SetOpacity(opacity);
}

void FadeToBlack::FadeOUT()
{

	if (!faded)
	{
		blackImage.SetOpacity(1.0f);
		faded = true;
	}

	opacity = 1.0f - fadeTimer;

	if (opacity <= 0.0f)
	{
		blackToFade = false;
		fadeTimer = 0.0f;
	}
	else
	{
		blackImage.SetOpacity(opacity);
	}
}

void FadeToBlack::FadeIN()
{

	if (!faded)
	{
		blackImage.SetOpacity(0.0f);
		faded = true;
	}

	opacity = fadeTimer;
	 
	if (opacity >= 1.0f)
	{
		fadeToBlack = false;
		fadeTimer = 0.0f;
	}
	else
	{
		blackImage.SetOpacity(opacity);
	}
}
