#pragma once
#include "ComponentUI.h"
#include "FfmpegVideoPlayer.h"

class VideoPlayerComponent : public ComponentUI
{
public:

	VideoPlayerComponent(GameObject* go);
	~VideoPlayerComponent();

	void CreateVideo(uint videoUID);

	void OnEditor() override;

	void Serialization(json& j);
	void DeSerialization(json& j);

	FfmpegVideoPlayer* GetVideoPlayer();

private:
	uint UID = 0;
	uint resourceUID = 0;
};

