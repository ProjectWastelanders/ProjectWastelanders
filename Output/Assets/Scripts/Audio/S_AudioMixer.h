#pragma once

#include "Globals.h"
#include "API/API.h"

static class S_AudioMixer
{
public:

	static void SetSFXVolume(float volume);
	static void SetMusicVolume(float volume);
	static void SetMasterVolume(float volume);

private:

	static float sfxVolume;
	static float musicVolume;
	static float masterVolume;
};

