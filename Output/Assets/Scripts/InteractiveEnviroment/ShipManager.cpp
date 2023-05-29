#include "ShipManager.h"
#include "ControlPanelShip.h"

HELLO_ENGINE_API_C ShipManager* CreateShipManager(ScriptToInspectorInterface* script)
{
	ShipManager* classInstance = new ShipManager();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ShipManager::Start()
{

	Game::FindGameObjectsWithTag("Ship", &Ships[0], 2);

}
void ShipManager::Update()
{
	
}

void ShipManager::SetShipActive(API_GameObject& ship)
{

	ship.SetActive(true);

	for (int i = 0; i < 2; i++)
	{
		if (Ships[i].GetTransform().GetGlobalPosition() == ship.GetTransform().GetGlobalPosition())
		{
			ShipActiveGO = &Ships[i];
		}
		else
		{
			ShipNotActiveGO = &Ships[i];
		}
	}

	ShipNotActiveGO->SetActive(false); 

}

void ShipManager::SetShipInactive(API_GameObject& ship)
{
	ship.SetActive(false);
}
