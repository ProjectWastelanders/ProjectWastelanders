#pragma once
#include "Component.h"
#include "ModuleAudio.h"

class AudioSourceComponent : public Component
{
public:
	AudioSourceComponent(GameObject* go);
	~AudioSourceComponent();

#ifdef STANDALONE
	void OnEditor() override;
#endif
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void PlayEvent();
	void StopEvent();

	void SetGameParameter(const char* paramName, float value);

private:
	static void FinishedEvent(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

	std::string audioEvent = "";
	AkGameObjectID akID = 0;
	AkPlayingID playingID = 0;
	bool isPlaying = false;
};

