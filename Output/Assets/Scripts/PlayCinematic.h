#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayCinematic : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_AudioSourceComponent audio;
	API_VideoPlayer video;
	API_UIButton closeButton;
};

