#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class FadeToBlack : HelloBehavior
{
public:

	void Init() override;

	void Start() override; 
	void Update() override;

	void FadeOUT();

	void FadeIN();

	API_UIImage blackImage;

	bool fadeToBlack = false;
	bool blackToFade = false;
	bool faded = false;

	float fadeTimer = 0.0f;
	float opacity;

};

