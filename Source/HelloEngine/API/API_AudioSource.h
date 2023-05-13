#pragma once

#include "Globals.h"
#include "API/API_GameObject.h"

class AudioSourceComponent;

namespace API
{
    class TO_API API_AudioSourceComponent
    {
    public:
        API_AudioSourceComponent();
        ~API_AudioSourceComponent();

        API_GameObject GetGameObject();

        void Play();

        void Stop();

        bool IsPlaying();

    private:
        AudioSourceComponent* GetComponent();
        void SetComponent(AudioSourceComponent* component);
    private:
        AudioSourceComponent* _audioSource = nullptr;

        friend class API_GameObject;
        friend class DragBoxAudioSourceComponent;
    };
}
