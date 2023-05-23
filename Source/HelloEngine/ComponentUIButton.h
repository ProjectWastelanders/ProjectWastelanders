#pragma once
#include "ComponentUI.h"
#include "ModuleResourceManager.h"

enum class ButtonState {
	NORMAL,
	HOVERED,
	ONPRESS,
	ONHOLD,
	NORMALBLOCKED,
	HOVEREDBLOCKED,
	ONPRESSBLOCKED,
	ONHOLDBLOCKED
};

class ComponentUIButton : public ComponentUI
{
	public:

	ComponentUIButton(GameObject* gameObject);
	~ComponentUIButton();

	void InputUpdate() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void UpdateGamePadInput(bool selected) override;

	ButtonState ChangeState(ButtonState State);

	void OnEnable();

	ButtonState State;
	ButtonState lastState;
	bool IsHold = false;

	bool isBlocked = false;

	bool isPress = true;
	bool isReleased = false;
	bool isHoveredFirstFrame = false;
private:
	double gameTimeCopy = 0;
	float colors[4] = { 0,0,0,0 };
	GameObject* uiObject;
	int textureIDIdle = -1;
	int textureIDHover = -1;
	int textureIDPress = -1;
	int textureIDIdleBlocked = -1;
	int textureIDHoverBlocked = -1;
	int textureIDHPressBlocked = -1;

	MeshRenderComponent* meshRenderer = nullptr;
	ResourceTexture* currentResource = nullptr;

	ResourceTexture* idleButton = nullptr;
	ResourceTexture* hoverButton = nullptr;
	ResourceTexture* pressButton = nullptr;
	ResourceTexture* idleblockedButton = nullptr;
	ResourceTexture* hoverblockedButton = nullptr;
	ResourceTexture* pressblockedButton = nullptr;

	bool AisPress = true;



#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

