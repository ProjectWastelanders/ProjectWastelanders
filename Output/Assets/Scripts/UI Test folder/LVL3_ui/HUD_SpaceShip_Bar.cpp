#include "HUD_SpaceShip_Bar.h"
HELLO_ENGINE_API_C HUD_SpaceShip_Bar* CreateHUD_SpaceShip_Bar(ScriptToInspectorInterface* script)
{
	HUD_SpaceShip_Bar* classInstance = new HUD_SpaceShip_Bar();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("SpaceShip GO", &classInstance->shipGO);
   

    script->AddDragBoxGameObject("HUD SpaceShip", &classInstance->shipHUD);
    script->AddDragBoxUIImage("HP SpaceShip UI IMG", &classInstance->shipHpBar);
    //script->AddDragBoxTextureResource("HP SpaceShip TEX", &classInstance->hpShipTex);
    
    script->AddCheckBox("aaaaaaaaaaaaaaaa", &classInstance->uwu);
   
	return classInstance;
}

void HUD_SpaceShip_Bar::Start()
{
    ship = (ReachTheSpaceship*)shipGO.GetScript("ReachTheSpaceship");
    if (!ship ) Console::Log("Missing ReachTheSpaceship on HUD_SpaceShip_Bar Script.");

    //shipHpBar.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(hpShipTex);
}
void HUD_SpaceShip_Bar::Update()
{
    
    if (ship!=nullptr)
    {
        uwu = true;
        if (ship->enabled)
        {
            shipHUD.SetActive(true);
            ShipBar(ship->currentHp/ship->maxHp);
        }
        else
        {
            shipHUD.SetActive(false);
        }
    }
}

void HUD_SpaceShip_Bar::ShipBar(float hp)
{
    
    shipHpBar.FillImage(hp);
}
