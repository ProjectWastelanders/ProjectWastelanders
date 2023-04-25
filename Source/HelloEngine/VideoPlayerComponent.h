#pragma once
#include "ComponentUI.h"
#include "FfmpegVideoPlayer.h"

class VideoPlayerComponent : public ComponentUI
{
public:

	VideoPlayerComponent(GameObject* go);
	~VideoPlayerComponent();

	void CreateVideo(std::string& path);

	void OnEditor() override;

	void Serialization(json& j);
	void DeSerialization(json& j);

	FfmpegVideoPlayer* GetVideoPlayer();

private:
	uint UID = 0;
};

