#pragma once

#include "Globals.h"
#include "API/API_GameObject.h"

class VideoPlayerComponent;

namespace API
{
    class TO_API API_VideoPlayer
    {
    public:
        API_VideoPlayer();
        ~API_VideoPlayer();

        API_GameObject GetGameObject();

        void Play();

        void Pause();

        void Stop();

        bool IsPlaying();

        bool VideoEnded();

        void Reset();

    private:
        VideoPlayerComponent* GetComponent();
        void SetComponent(VideoPlayerComponent* component);
    private:
        VideoPlayerComponent* _videoPlayer = nullptr;

        friend class API_GameObject;
        friend class DragBoxVideoPlayerComponent;
    };
}


