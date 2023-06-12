#include "Headers.h"
#include "API_VideoPlayer.h"
#include "VideoPlayerComponent.h"

API::API_VideoPlayer::API_VideoPlayer()
{
}

API::API_VideoPlayer::~API_VideoPlayer()
{
}

API::API_GameObject API::API_VideoPlayer::GetGameObject()
{
    if (!_videoPlayer)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR VidePlayer");
        return API_GameObject();
    }
    API_GameObject returnGO;
    returnGO.SetGameObject(_videoPlayer->GetGameObject());
    return returnGO;
}

void API::API_VideoPlayer::Play()
{
    if (!_videoPlayer)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR VidePlayer");
        return;
    }
    _videoPlayer->GetVideoPlayer()->isPlaying = true;
}

void API::API_VideoPlayer::Pause()
{
    if (!_videoPlayer)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR VidePlayer");
        return;
    }
    _videoPlayer->GetVideoPlayer()->isPlaying = false;
}

void API::API_VideoPlayer::Stop()
{
    if (!_videoPlayer)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR VidePlayer");
        return;
    }
    _videoPlayer->GetVideoPlayer()->isPlaying = false;
    _videoPlayer->GetVideoPlayer()->video->ResetVideo();
}

bool API::API_VideoPlayer::IsPlaying()
{
    return _videoPlayer->GetVideoPlayer()->isPlaying;
}

bool API::API_VideoPlayer::VideoEnded()
{
    return _videoPlayer->GetVideoPlayer()->video->videoEnded;
}

VideoPlayerComponent* API::API_VideoPlayer::GetComponent()
{
    return _videoPlayer;
}

void API::API_VideoPlayer::SetComponent(VideoPlayerComponent* component)
{
    _videoPlayer = component;
}
