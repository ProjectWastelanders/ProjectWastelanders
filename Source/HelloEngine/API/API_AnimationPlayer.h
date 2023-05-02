#pragma once

#include "Globals.h"
#include "API/API_GameObject.h"

class AnimationComponent;

namespace API
{
	class TO_API API_AnimationPlayer
	{
	public:
		API_AnimationPlayer();
		~API_AnimationPlayer();

		API_GameObject GetGameObject();

		void Play();

		void Pause();

		void Resume();

		void Stop();

		bool IsPlaying();

		void ChangeAnimation(uint animationResourceUID);

		void SetLoop(bool loop);

		void SetStayLast(bool stayLast);

	private:
		AnimationComponent* GetComponent();
		void SetComponent(AnimationComponent* component);
	private:
		AnimationComponent* _animation = nullptr;

		friend class API_GameObject;
		friend class DragBoxAnimationPlayer;
	};
}
