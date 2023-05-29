#include "ControlPanelShip.h"
#include "../FadeToBlack.h"
#include "ShipManager.h"

HELLO_ENGINE_API_C ControlPanelShip* CreateControlPanelShip(ScriptToInspectorInterface* script)
{
	ControlPanelShip* classInstance = new ControlPanelShip();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("Ship", &classInstance->Ship);
	script->AddDragBoxGameObject("Black Image", &classInstance->fadeToBlackGO);
    script->AddDragBoxGameObject("ShipManager", &classInstance->ShipManagerGO);
    script->AddCheckBox("Set Ship Here", &classInstance->ShipActivated);

	return classInstance;
}

void ControlPanelShip::Start()
{
	fadeToBlackRef = (FadeToBlack*)fadeToBlackGO.GetScript("FadeToBlack");
	if (!fadeToBlackRef) Console::Log("fadeToBlackRef Missing in ControlPanelShip.");

    ShipManagerRef = (ShipManager*)ShipManagerGO.GetScript("ShipManager");
    if (!ShipManagerRef) Console::Log("fadeToBlackRef Missing in ControlPanelShip.");

    if (ShipActivated)
    {
        ShipManagerRef->SetShipActive(Ship);
    }
    else
    {
        ShipManagerRef->SetShipInactive(Ship);
    }
}
void ControlPanelShip::Update()
{
}

void ControlPanelShip::OnCollisionStay(API_RigidBody other)
{
    std::string detectionName = other.GetGameObject().GetName();
    if (detectionName == "Player")
    {

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_REPEAT)
        {
            timerToShip += Time::GetDeltaTime();

            if (timerToShip >= 0.98f)
            {
                ShipManagerRef->SetShipActive(Ship);
            }
            else
            {
                fadeToBlackRef->fadeToBlack = true;
            }
        }
        else if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP)
        {
            fadeToBlackRef->fadeToBlack = false;
            fadeToBlackRef->blackToFade = true;
            timerToShip = 0.0f;
        }


    }
}
