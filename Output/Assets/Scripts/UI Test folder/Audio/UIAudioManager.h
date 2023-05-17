#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIText.h"

class UIAudioManager : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	std::vector<API_UIButton> sceneButtons;

	API_UIText masterText;
	API_UIText sfxText;
	API_UIText musicText;

	API_UISlider masterSlider;
	API_UISlider sfxSlider;
	API_UISlider musicSlider;
};

