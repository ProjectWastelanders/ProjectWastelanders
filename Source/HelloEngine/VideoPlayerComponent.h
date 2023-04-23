#pragma once
#include "Component.h"
class VideoPlayerComponent : public Component
{
public:

	VideoPlayerComponent(GameObject* go);
	~VideoPlayerComponent();

	void Serialization(json& j);
	void DeSerialization(json& j);
};

