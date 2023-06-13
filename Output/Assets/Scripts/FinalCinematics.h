#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class FinalCinematics : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_VideoPlayer cinematic4;
	API_VideoPlayer cinematic5;
	API_VideoPlayer credits;
	API_AudioSourceComponent audio4;
	API_AudioSourceComponent audio5;
	API_AudioSourceComponent audioCredits;

	uint currentVideoNum = 0;
	API_VideoPlayer currentVideo;
};

