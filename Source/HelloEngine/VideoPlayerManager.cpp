#include "Headers.h"
#include "VideoPlayerManager.h"

std::map<uint, FfmpegVideoPlayer> VideoPlayerManager::videoPlayers;

VideoPlayerManager::VideoPlayerManager()
{
}

VideoPlayerManager::~VideoPlayerManager()
{
}

void VideoPlayerManager::Update()
{
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
