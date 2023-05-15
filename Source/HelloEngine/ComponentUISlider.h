#pragma once
#include "ComponentUI.h"

enum class SliderState {
	NORMAL,
	ONPRESS,
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

	SliderState State;


private:

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	ImWindowGame* _gameWindow = nullptr;

	float numMax = 1;
	float numMin = 0;

	bool isFocused = false;
	float mousePosX = 0;

	float widthBar;
	float widthBarAux;

	int perCent = 0;

	float NormalizedPos = 0;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

