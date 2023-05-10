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

private:
	std::string audioEvent = "";
	AkGameObjectID akID = 0;
	AkPlayingID playingID = 0;
};

