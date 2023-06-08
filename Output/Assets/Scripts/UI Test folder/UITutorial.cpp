#include "UITutorial.h"

UITutorial::UITutorial(API_GameObject* tutorials, uint count)
{
	for (int i = 0; i < count; ++i)
	{
		tutorialScreens[i] = *tutorials;
		tutorials++;
	}
	totalScreens = count;
}

bool UITutorial::NextScreen()
{
	if (totalScreens == currentScreen + 1)
	{
		tutorialScreens[currentScreen].SetActive(false);
		return false;
	}
	++currentScreen;
	for (int i = 0; i < 4; ++i)
	{
		tutorialScreens[i].SetActive(i == currentScreen);
	}
	return true;
}

void UITutorial::Unable()
{
	for (int i = 0; i < 4; ++i)
	{
		tutorialScreens[i].SetActive(false);
	}
}
