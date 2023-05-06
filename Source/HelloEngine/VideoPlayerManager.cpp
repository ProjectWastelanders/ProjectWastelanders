#include "Headers.h"
#include "VideoPlayerManager.h"
#include "VideoPlayerComponent.h"
#include "ModuleResourceManager.h"

std::map<uint, VideoPlayer> VideoPlayerManager::videoPlayers;
std::map<uint, VideoPlayerComponent*> VideoPlayerManager::videoComponents;

VideoPlayerManager::VideoPlayerManager()
{
}

VideoPlayerManager::~VideoPlayerManager()
{
}

void VideoPlayerManager::Update()
{
    for (auto& video : videoPlayers)
    {
        if (video.second.isPlaying)
            video.second.video->Update();
    }
}

uint VideoPlayerManager::AddVideoPlayer(uint videoResourceUID)
{
    uint UID = HelloUUID::GenerateUUID();

    if (!ModuleResourceManager::S_IsResourceCreated(videoResourceUID))
        return 0;

    ResourceVideo* videoRes = (ResourceVideo*)ModuleResourceManager::S_LoadResource(videoResourceUID);
    videoPlayers[UID].video = videoRes->video;

    return UID;
}

VideoPlayer* VideoPlayerManager::GetVideoPlayer(uint UID)
{
    if (videoPlayers.count(UID) != 0)
    {
        return &videoPlayers[UID];
    }
    return nullptr;
}

void VideoPlayerManager::RemoveVideoPlayer(uint UID)
{
    if (videoPlayers.count(UID) != 0)
    {
        videoPlayers.erase(UID);
    }
}
