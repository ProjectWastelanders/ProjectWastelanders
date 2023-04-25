#include "Headers.h"
#include "VideoPlayerManager.h"
#include "VideoPlayerComponent.h"

std::map<uint, FfmpegVideoPlayer> VideoPlayerManager::videoPlayers;
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
        if (video.second.Update())
        {
            videoComponents[video.first].
        }
    }
}

uint VideoPlayerManager::AddVideoPlayer(std::string& path)
{
    uint UID = HelloUUID::GenerateUUID();

    videoPlayers[UID] = FfmpegVideoPlayer(path.c_str());

    return UID;
}

FfmpegVideoPlayer* VideoPlayerManager::GetVideoPlayer(uint UID)
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
        videoPlayers[UID].CleanUp();
        videoPlayers.erase(UID);
    }
}
