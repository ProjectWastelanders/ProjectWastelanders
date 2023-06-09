#include "Headers.h"
#include "API_ParticleSystem.h"
#include "ModuleLayers.h"
#include "ParticleSystemComponent.h"

API::API_ParticleSystem::API_ParticleSystem()
{
}

API::API_ParticleSystem::~API_ParticleSystem()
{
}

API::API_GameObject API::API_ParticleSystem::GetGameObject()
{
    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. GetGameObject()");
        return *ModuleLayers::emptyAPIGameObject;
    }

    API_GameObject goToReturn;
    goToReturn.SetGameObject(_particleSystem->GetGameObject());

    return goToReturn;
}

void API::API_ParticleSystem::Play()
{
    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. Play()");
        return;
    }
    _particleSystem->ResetEmitterTimers();
    _particleSystem->SetStopEmitting(false);
    _particleSystem->SetPlayOnGame(true);

}

void API::API_ParticleSystem::Pause()
{
    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. Pause()");
        return;
    }
    _particleSystem->SetPlayOnGame(false);
}

void API::API_ParticleSystem::StopEmitting()
{
    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. StopEmitting()");
        return;
    }

    _particleSystem->ResetEmitterTimers();
    _particleSystem->SetStopEmitting(true);
}

void API::API_ParticleSystem::Stop()
{
    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. Stop()");
        return;
    }

    if (_particleSystem->GetPlayOnGame())
    {
        _particleSystem->SetPlayOnGame(false);
        _particleSystem->GetParticleSystemEmitter().SetPlayOnAwake(false);
        _particleSystem->GetParticleSystemEmitter().ResetEmitter();
    }
}

void API::API_ParticleSystem::SetInitialSpeed(API_Vector3 speed)
{
    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. SetInitialSpeed()");
        return;
    }
    _particleSystem->particleProps.speed = speed;
}

void API::API_ParticleSystem::SetRotation(API_Vector3 rotation)
{

    if (_particleSystem == nullptr)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR API_ParicleSystem. SetInitialSpeed()");
        return;
    }
    _particleSystem->particleProps.rotation = rotation;

}

ParticleSystemComponent* API::API_ParticleSystem::GetComponent()
{

    return _particleSystem;

}

void API::API_ParticleSystem::SetComponent(ParticleSystemComponent* component)
{

    _particleSystem = component;

}
