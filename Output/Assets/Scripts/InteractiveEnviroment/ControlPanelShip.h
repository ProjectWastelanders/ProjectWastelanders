#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class FadeToBlack;
class ShipManager;

class ControlPanelShip : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void OnCollisionStay(API_RigidBody other);

public:
	API_GameObject Ship;
	API_GameObject ShipManagerGO;
	ShipManager* ShipManagerRef;

	API_GameObject fadeToBlackGO;
	FadeToBlack* fadeToBlackRef;

	float timerToShip = 0.0f;

	bool ShipActivated = false;
	bool AudioActivated = true;
	bool AudioActivated2 = true;

};

