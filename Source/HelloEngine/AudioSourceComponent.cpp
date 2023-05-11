#include "Headers.h"
#include "AudioSourceComponent.h"
#include "GameObject.h"
#include "ModuleLayers.h"

AudioSourceComponent::AudioSourceComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::AUDIO_SOURCE;

	// Create AKGameObject and get ID
	akID = ModuleAudio::RegisterGameObject(_gameObject->GetID());
}

AudioSourceComponent::~AudioSourceComponent()
{
}

#ifdef STANDALONE
void AudioSourceComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Audio Source", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputText("Event:", &audioEvent);
		if (ImGui::Button("Play"))
		{
			PlayEvent();
		}
		if (ImGui::Button("Stop"))
		{
			StopEvent();
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);

}
#endif
void AudioSourceComponent::Serialization(json& j)
{
	json _j;
	_j["Type"] = _type;
	_j["Enabled"] = _isEnabled;

	_j["AudioEvent"] = audioEvent;

	j["Components"].push_back(_j);
}

void AudioSourceComponent::DeSerialization(json& j)
{
	audioEvent = j["AudioEvent"];
	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}

void AudioSourceComponent::PlayEvent()
{
	AkCallbackFunc callbakc = &FinishedEvent;
	playingID = AK::SoundEngine::PostEvent(audioEvent.c_str(), akID, 0U, &FinishedEvent);
	if (playingID == 0)
	{
		Console::S_Log("Error at reproducing audio event: " + audioEvent);
	}
	else
		isPlaying = true;
}

void AudioSourceComponent::StopEvent()
{
	AK::SoundEngine::StopPlayingID(playingID);
	isPlaying = false;
}

void AudioSourceComponent::SetGameParameter(const char* paramName, float value)
{
	AK::SoundEngine::SetRTPCValue(paramName, value, this->akID);
	
}

void AudioSourceComponent::FinishedEvent(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	if (in_eType == AkCallbackType::AK_EndOfEvent)
	{
		if (ModuleLayers::gameObjects.count(in_pCallbackInfo->gameObjID) != 0)
		{
			GameObject* go = ModuleLayers::gameObjects[in_pCallbackInfo->gameObjID];
			AudioSourceComponent* audio = go->GetComponent<AudioSourceComponent>();
			if (audio != nullptr)
			{
				audio->isPlaying = false;
			}
		}
	}
}
