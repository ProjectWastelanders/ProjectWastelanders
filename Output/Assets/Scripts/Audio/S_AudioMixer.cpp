#include "S_AudioMixer.h"

float S_AudioMixer::sfxVolume = 100.0f;
float S_AudioMixer::musicVolume = 100.0f;
float S_AudioMixer::masterVolume = 1.0f;

void S_AudioMixer::SetSFXVolume(float volume)
{
	sfxVolume = volume;
	Audio::SetGlobalGameParameter("SFXVolume", sfxVolume * masterVolume);
	API_QuickSave::SetFloat("SFXVolume", sfxVolume);
}

void S_AudioMixer::SetMusicVolume(float volume)
{
	musicVolume = volume;
	Audio::SetGlobalGameParameter("MusicVolume", musicVolume * masterVolume);
	API_QuickSave::SetFloat("MusicVolume", musicVolume);
}

void S_AudioMixer::SetMasterVolume(float volume)
{
	masterVolume = volume;
	Audio::SetGlobalGameParameter("SFXVolume", sfxVolume * masterVolume);
	Audio::SetGlobalGameParameter("MusicVolume", musicVolume * masterVolume);

	API_QuickSave::SetFloat("MasterVolume", masterVolume);

}
