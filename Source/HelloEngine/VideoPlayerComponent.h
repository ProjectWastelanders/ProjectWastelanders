#pragma once
#include "ComponentUI.h"
#include "VideoPlayerManager.h"

class VideoPlayerComponent : public ComponentUI
{
public:

	VideoPlayerComponent(GameObject* go);
	~VideoPlayerComponent();

	void CreateVideo(uint videoUID);

	void OnEditor() override;

	void Serialization(json& j);
	void DeSerialization(json& j);

	VideoPlayer* GetVideoPlayer();

private:
	uint UID = 0;
	uint resourceUID = 0;
	float fps = 24.0f;

	friend class VideoPlayerManager;
};

