#include "Headers.h"
#include "API_AudioSource.h"
#include "AudioSourceComponent.h"

API::API_AudioSourceComponent::API_AudioSourceComponent()
{
}

API::API_AudioSourceComponent::~API_AudioSourceComponent()
{
}

API::API_GameObject API::API_AudioSourceComponent::GetGameObject()
{
    if (!_audioSource)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR AudioSourceComponent");
        return API_GameObject();
    }
    API_GameObject returnGO;
    returnGO.SetGameObject(_audioSource->GetGameObject());
    return returnGO;
}

void API::API_AudioSourceComponent::Play()
{
    if (!_audioSource)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR AudioSourceComponent");
        return;
    }
    _audioSource->PlayEvent();
}

void API::API_AudioSourceComponent::Stop()
{
    if (!_audioSource)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR AudioSourceComponent");
        return;
    }
    _audioSource->StopEvent();
}

bool API::API_AudioSourceComponent::IsPlaying()
{
    if (!_audioSource)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR AudioSourceComponent");
        return false;
    }
	return _audioSource->IsPlaying();
}

AudioSourceComponent* API::API_AudioSourceComponent::GetComponent()
{
	return _audioSource;
}

void API::API_AudioSourceComponent::SetComponent(AudioSourceComponent* component)
{
    _audioSource = component;
}
