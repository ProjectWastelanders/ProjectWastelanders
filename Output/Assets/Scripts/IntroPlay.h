#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class IntroPlay : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_VideoPlayer video;
	API_AudioSourceComponent audio;
};

