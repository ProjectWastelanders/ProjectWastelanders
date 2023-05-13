#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class UIAudioManager : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	std::vector<API_UIButton> sceneButtons;
};

