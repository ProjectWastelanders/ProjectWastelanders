#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayCreditsMainMenu : HelloBehavior
{
public:

	void Update() override;
	void PlayCinematic();

	API_VideoPlayer video;
	API_AudioSourceComponent audio;
	uint audioPlayingID = 0;
	bool playing = false;
};

