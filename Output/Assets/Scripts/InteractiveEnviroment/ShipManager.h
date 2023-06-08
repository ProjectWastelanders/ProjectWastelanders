#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ControlPanelShip;

class ShipManager : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void SetShipActive(API_GameObject& ship);

	void SetShipInactive(API_GameObject& ship);

	API_GameObject Ships[2];


	API_GameObject* ShipActiveGO;
	API_GameObject* ShipNotActiveGO;
};

