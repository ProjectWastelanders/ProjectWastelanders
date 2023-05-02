#pragma once
#include "ComponentUI.h"

class ComponentUIImage : public ComponentUI
{
	public:

	ComponentUIImage(GameObject* gameObject);
	~ComponentUIImage();

	void InputUpdate() override;

	void SetFill(float fill);
	void SetOpacity(float opacity);

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;
	float _fillImage;
	float _maxScale;

private: 
	float _auxFillImage;
	ImWindowGame* _gameWindow = nullptr;

	std::string comboNames[4] = { "Left To Right", "Right To Left", "Up To Down", "Down To Up" };

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

