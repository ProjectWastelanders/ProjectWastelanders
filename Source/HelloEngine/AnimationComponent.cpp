#include "Headers.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"
#include "LayerEditor.h"
#include "SkinnedMeshRenderComponent.h"

AnimationComponent::AnimationComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::ANIMATION_PLAYER;
	isPlaying = false;
	isPaused = false;
	isLoop = true;
	isStayLast = false;

}

AnimationComponent::~AnimationComponent()
{
	LayerGame::S_RemoveAnimationComponent(this);
}
#ifdef STANDALONE
void AnimationComponent::OnEditor()
{
	bool created = true;

	if (ImGui::CollapsingHeader("Animation Player", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		AnimationDropArea();

		if (_resource != nullptr)
		{

			ImGui::InputFloat("Animation Speed", &speedMultiplier);

			if (isPlaying)
			{
				if (ImGui::Button("Stop"))
				{
					StopAnimation();
				}

				if (!isPaused)
				{
					if (ImGui::Button("Pause"))
					{
						PauseAnimation();
					}
				}
				else
				{
					if (ImGui::Button("Resume"))
					{
						ResumeAnimation();
					}
				}
				
			}
			else
			{
				if (ImGui::Button("Play"))
				{
					PlayAnimation();
				}
			}
			
			ImGui::Checkbox("Loop", &isLoop);
			ImGui::Checkbox("Stay last", &isStayLast);

			if (isPlaying)
			{
				ImGui::Text(std::to_string(currentTime / 1000).c_str());
				ImGui::Text(std::to_string(animDuration / 1000).c_str());
			}
		}

	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}
#endif
void AnimationComponent::AnimationDropArea()
{
	ImGui::NewLine();

	std::string dragText = "Drag animation here";
	if (_resource != nullptr) dragText = _resource->resourcePath;

	ImGui::Button(dragText.c_str());

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Animation"))
		{
			const uint* drop = (uint*)payload->Data;

			if (_resource != nullptr) _resource->Dereference();
			//_resourceUID = _resource->UID;
			_resource = (ResourceAnimation*)ModuleResourceManager::S_LoadResource(*drop);


			std::string popUpmessage = "Loaded Animation: ";
			LayerEditor::S_AddPopUpMessage(popUpmessage);
		}
	}
}


void AnimationComponent::ChangeAnimation(uint animUID)
{
	StopAnimation();
	if (_resource != nullptr)
		_resource->Dereference();

	_resource = (ResourceAnimation*)ModuleResourceManager::S_LoadResource(animUID);
}

void AnimationComponent::PlayAnimation()
{
	if (_resource == nullptr) return;
	LayerGame::S_AddAnimationComponent(this);

	isPlaying = true;
	currentTime = 0;
	animDuration = _resource->animation.durationTicks;
	
	SkinnedMeshRenderComponent* skin = _gameObject->GetComponent<SkinnedMeshRenderComponent>();
	if (skin)
	{
		skin->hasAnim = true;
	}
}

void AnimationComponent::StopAnimation()
{
	isPlaying = false;
	isPaused = false;
	LayerGame::S_RemoveAnimationComponent(this);

	animDuration = 0;
	currentTime = 0;

	SkinnedMeshRenderComponent* skin = _gameObject->GetComponent<SkinnedMeshRenderComponent>();
	if (skin)
	{
		skin->hasAnim = false;
	}
}

void AnimationComponent::PauseAnimation()
{
	isPaused = true;
}

void AnimationComponent::ResumeAnimation()
{
	isPaused = false;
}

void AnimationComponent::UpdateAnimation()
{
	OPTICK_EVENT();
	if (_resource == nullptr) return;
	if (!isPlaying) return;

	SkinnedMeshRenderComponent* skin = _gameObject->GetComponent<SkinnedMeshRenderComponent>();

	if (skin != nullptr && skin->HasBones())
	{
		if (!isPaused)
		{
			Animation3D& animation = _resource->animation;
			float deltaTime = EngineTime::GameDeltaTime();

			currentTime += (animation.ticksPerSecond * speedMultiplier) * deltaTime; 

			//If animation ends and it should stay on the last keyframe
			if (currentTime > animDuration && isStayLast)
			{
				return;
			}

			//If animation ends and it isn't a loop, stop animation
			if (currentTime > animDuration && !isLoop)
			{
				StopAnimation();
				return;
			}

			currentTime = fmod(currentTime, animDuration);
		}

		skin->UpdateBones(&_resource->animation, currentTime);
	}
}

void AnimationComponent::Serialization(json& j)
{
	json _j;
	
	_j["Type"] = _type;

	if (_resource != nullptr)
	{
		_j["AnimationUID"] = _resource->UID;
	}
	else
	{
		_j["AnimationUID"] = 0;
	}

	_j["isLoop"] = isLoop;
	_j["isStayLast"] = isStayLast;

	_j["Enabled"] = _isEnabled;

	j["Components"].push_back(_j);
}

void AnimationComponent::DeSerialization(json& j)
{
	ResourceAnimation* res = (ResourceAnimation*)ModuleResourceManager::S_LoadResource(j["AnimationUID"]);

	if (res != nullptr)
	{
		_resource = res;
	}

	isLoop = j["isLoop"];
	isStayLast = j["isStayLast"];



	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}