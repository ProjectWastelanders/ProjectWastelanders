#pragma once
#include "ComponentUI.h"
#include "ModuleResourceManager.h"

enum class CheckboxState {
	NORMAL,
	HOVERED,
	ONPRESS,
	NORMALACTIVE,
	HOVEREDACTIVE,
	ONPRESSACTIVE
};

class ComponentUICheckbox : public ComponentUI
{
	public:

	ComponentUICheckbox(GameObject* gameObject);
	~ComponentUICheckbox();

	void InputUpdate() override;
	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void UpdateGamePadInput(bool selected) override;

	CheckboxState ChangeState(CheckboxState State);

	bool checkActive = false;
	CheckboxState State;
private:
	float colors[4] = { 0,0,0,0 };

	int textureIDIdleCB = -1;
	int textureIDHoverCB = -1;
	int textureIDPressCB = -1;
	int textureIDActiveCB = -1;
	int textureIDHoverActiveCB = -1;
	int textureIDPressActiveCB = -1;

	ResourceTexture* idleCB = nullptr;
	ResourceTexture* hoverCB = nullptr;
	ResourceTexture* pressCB = nullptr;
	ResourceTexture* activeCB = nullptr;
	ResourceTexture* hoverActiveCB = nullptr;
	ResourceTexture* pressActiveCB = nullptr;

#ifdef STANDALONE
	void OnEditor() override;

#endif // STANDALONE
};

