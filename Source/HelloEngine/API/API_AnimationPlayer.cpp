#include "Headers.h"
#include "ModuleLayers.h"
#include "API_AnimationPlayer.h"
#include "AnimationComponent.h"

API::API_AnimationPlayer::API_AnimationPlayer()
{
}

API::API_AnimationPlayer::~API_AnimationPlayer()
{
}

API::API_GameObject API::API_AnimationPlayer::GetGameObject()
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_animation->GetGameObject());
	return returnGO;
}

void API::API_AnimationPlayer::Play()
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->PlayAnimation();
}

void API::API_AnimationPlayer::Pause()
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->PauseAnimation();
}

void API::API_AnimationPlayer::Resume()
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->ResumeAnimation();
}

void API::API_AnimationPlayer::Stop()
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->StopAnimation();
}

bool API::API_AnimationPlayer::IsPlaying()
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return false;
	}
	_animation->IsPlaying();
}

void API::API_AnimationPlayer::ChangeAnimation(uint animationResourceUID)
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->ChangeAnimation(animationResourceUID);
}

void API::API_AnimationPlayer::SetLoop(bool loop)
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->SetLoop(loop);
}

void API::API_AnimationPlayer::SetStayLast(bool stayLast)
{
	if (!_animation)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR AnimationPlayer");
		return;
	}
	_animation->SetStayLast(stayLast);
}

AnimationComponent* API::API_AnimationPlayer::GetComponent()
{
    return _animation;
}

void API::API_AnimationPlayer::SetComponent(AnimationComponent* component)
{
	_animation = component;
}
