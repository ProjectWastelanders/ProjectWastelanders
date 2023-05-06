#include "Headers.h"
#include "AudioSourceComponent.h"
#include "GameObject.h"

AudioSourceComponent::AudioSourceComponent(GameObject* go) : Component(go)
{
	_type = Component::Type::AUDIO_SOURCE;

	// Create AKGameObject and get ID
	akID = ModuleAudio::RegisterGameObject(_gameObject->GetID());
}

AudioSourceComponent::~AudioSourceComponent()
{
}

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
	playingID = AK::SoundEngine::PostEvent(audioEvent.c_str(), akID);
	if (playingID == 0)
	{
		Console::S_Log("Error at reproducing audio event: " + audioEvent);
	}
}

void AudioSourceComponent::StopEvent()
{
	AK::SoundEngine::StopPlayingID(playingID);
}
