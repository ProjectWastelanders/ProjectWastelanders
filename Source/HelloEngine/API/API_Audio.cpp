#include "Headers.h"	
#include "API_Audio.h"
#include "ModuleAudio.h"

uint API::Audio::Event(const char* eventName)
{
	return ModuleAudio::S_ProduceEvent(eventName);
}

uint API::Audio::Event(uint eventID)
{
	return ModuleAudio::S_ProduceEvent(eventID);
}

void API::Audio::StopEvent(uint playID)
{
	AK::SoundEngine::StopPlayingID(playID);
}
