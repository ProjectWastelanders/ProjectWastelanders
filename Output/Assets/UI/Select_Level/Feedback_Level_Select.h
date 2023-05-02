#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"


class Feedback_Level_Select : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	//void OnCollisionStay(API::API_RigidBody other) override;
	void OnCollisionEnter(API::API_RigidBody other) override;
	void OnCollisionExit(API::API_RigidBody other) override;

	void OnCharge();
	void SetCurrentLevel();

	API_UIImage chargingBar;
	API_UIImage isSelected;

	API_UIInput chargingConjunt;

	API_UICheckBox level1;
	API_UICheckBox level2;
	API_UICheckBox level3;
	API_UICheckBox level4;
	API_UICheckBox currentLevel;

	int indexLevles;

	float openSelecTime;
	float maxOpenSelectTime = 1.0f;

	bool selecting;
};

