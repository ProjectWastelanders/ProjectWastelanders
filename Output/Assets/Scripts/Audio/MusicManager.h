#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class MusicManager : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void PlayCasette(int casetteNum);

	API_AudioSourceComponent mainMusic;
	API_AudioSourceComponent casetteMusic[3];

	bool casettePlaying = false;
	int currentPlayingCassette = 0;
};

