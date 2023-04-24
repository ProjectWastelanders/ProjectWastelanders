#include "Headers.h"
#include "VideoPlayerComponent.h"
#include "GameObject.h"
#include "VideoPlayerManager.h"

VideoPlayerComponent::VideoPlayerComponent(GameObject* go) : Component(go)
{
}

VideoPlayerComponent::~VideoPlayerComponent()
{
}

void VideoPlayerComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Video Player", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (UID == 0)
		{
			// DRAG VIDEO 
		}
		else
		{
			// Play/Pause/Stop
			// Change video properties.
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void VideoPlayerComponent::Serialization(json& j)
{
}

void VideoPlayerComponent::DeSerialization(json& j)
{
}

FfmpegVideoPlayer* VideoPlayerComponent::GetVideoPlayer()
{
	if (UID == 0)
		return nullptr;
	return VideoPlayerManager::GetVideoPlayer(UID);
}
