#pragma once
#include "ComponentUI.h"

enum class SliderState {
	NORMAL,
	ONPRESS,
	HOVER,
	ONHOLD,
	ONUNHOLD
};

class ComponentUISlider : public ComponentUI
{
public:
	ComponentUISlider(GameObject* gameObject);
	~ComponentUISlider();

	void InputUpdate() override;

	void UpdateGamePadInput(bool selected) override;

	void CalculPerCent();
	void CalculNormalize();

	void SetValue(float value);

	SliderState State;

	int perCent = 0;
	float mousePosX = 0;

private:

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	ImWindowGame* _gameWindow = nullptr;

	float numMax = 1;
	float numMin = 0;

	bool isFocused = false;

	float widthBar;
	float widthBarAux;

	float NormalizedPos = 0;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

