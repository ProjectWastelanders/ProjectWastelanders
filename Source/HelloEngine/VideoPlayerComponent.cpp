#include "Headers.h"
#include "VideoPlayerComponent.h"
#include "GameObject.h"
#include "VideoPlayerManager.h"
#include "TextureComponent.h"

VideoPlayerComponent::VideoPlayerComponent(GameObject* go) : ComponentUI(go)
{
}

VideoPlayerComponent::~VideoPlayerComponent()
{
	if (UID != 0)
	{
		VideoPlayerManager::RemoveVideoPlayer(UID);
		if (ModuleResourceManager::S_IsResourceCreated(resourceUID))
		{
			ResourceVideo* videoRes = (ResourceVideo*)ModuleResourceManager::resources[resourceUID];
			videoRes->Dereference();
		}
	}

}

void VideoPlayerComponent::CreateVideo(uint videoUID)
{
	if (ModuleResourceManager::S_IsResourceCreated(videoUID))
	{
		resourceUID = videoUID;
		UID = VideoPlayerManager::AddVideoPlayer(resourceUID);
	}
	this->_material->ChangeTexture(GetVideoPlayer()->video->GetOpenGLTexture());
	fps = GetVideoPlayer()->video->GetFPS();
}

void VideoPlayerComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Video Player", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (UID == 0)
		{
			ImGui::TextColored(ImVec4(1, 0, 1, 1), "Drag a video here");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Video"))
				{
					const uint* drop = (uint*)payload->Data;

					CreateVideo(*drop);

				}
				ImGui::EndDragDropTarget();
			}
		}
		else
		{
			if (!GetVideoPlayer()->isPlaying)
			{
				if (ImGui::Button("Play"))
				{
					GetVideoPlayer()->isPlaying = true;
				}
			}
			else
			{
				if (ImGui::Button("Pause"))
				{
					GetVideoPlayer()->isPlaying = false;
				}
				if (ImGui::Button("Stop"))
				{
					GetVideoPlayer()->video->ResetVideo();
					GetVideoPlayer()->isPlaying = false;
				}
			}
		
			if (ImGui::InputFloat("Frames per second", &fps))
			{
				GetVideoPlayer()->video->SetFPS(fps);
			}
			if (ImGui::Button("Set to source framerate"))
			{
				GetVideoPlayer()->video->SetFPS(GetVideoPlayer()->video->GetFPS());
				fps = GetVideoPlayer()->video->GetFPS();
			}

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

VideoPlayer* VideoPlayerComponent::GetVideoPlayer()
{
	if (UID == 0)
		return nullptr;
	return VideoPlayerManager::GetVideoPlayer(UID);
}
