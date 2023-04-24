#pragma once
#include "Component.h"
#include "FfmpegVideoPlayer.h"

class VideoPlayerComponent : public Component
{
public:

	VideoPlayerComponent(GameObject* go);
	~VideoPlayerComponent();

	void OnEditor() override;

	void Serialization(json& j);
	void DeSerialization(json& j);

	FfmpegVideoPlayer* GetVideoPlayer();

private:
	uint UID = 0;
};

