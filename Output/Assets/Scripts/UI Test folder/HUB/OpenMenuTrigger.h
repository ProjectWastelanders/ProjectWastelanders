#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class OpenMenuTrigger : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionStay(API_RigidBody other) override;
	void OnCollisionExit(API_RigidBody other) override;

	API_GameObject menu;
	API_GameObject secondaryMenu;
	API_GameObject buttonToPressImage;

	bool isPlayerInsideTrigger = false;
};

