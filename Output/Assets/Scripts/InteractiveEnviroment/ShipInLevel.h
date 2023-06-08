#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class FadeToBlack;
class PlayerStorage;
class ControlPanelShip;

class ShipInLevel : HelloBehavior
{
public:
	void Init() override;
	void Start() override; 
	void Update() override;

	void OnCollisionEnter(API::API_RigidBody other);

	void OnCollisionStay(API::API_RigidBody other);

	API_GameObject fadeToBlackGO;
	FadeToBlack* fadeToBlackRef;

	API_GameObject controlPanelShipGO;
	ControlPanelShip* controlPanelShipRef;

	API_GameObject playerGO;
	PlayerStorage* playerStorageRef;

	API_Transform positionToReturn;

	float timerTotp = 0.0f;
	int lvlIndex = 1;

};

