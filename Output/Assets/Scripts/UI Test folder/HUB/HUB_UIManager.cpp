#include "HUB_UIManager.h"
#include "../../Player/PlayerMove.h"

bool HUB_UIManager::openedPanel = false;
PlayerMove* HUB_UIManager::player = nullptr;

HELLO_ENGINE_API_C HUB_UIManager* CreateHUB_UIManager(ScriptToInspectorInterface* script)
{
	HUB_UIManager* classInstance = new HUB_UIManager();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void HUB_UIManager::Start()
{
	player = (PlayerMove*)Game::FindGameObject("Player").GetScript("PlayerMove");
	ClosePanel();
}

void HUB_UIManager::OpenPanel()        
{
	openedPanel = true;
	if (player)
		player->openingChest = true; 
}

void HUB_UIManager::ClosePanel()
{
	openedPanel = false;
	if (player)
		player->openingChest = false;

}

bool HUB_UIManager::IsPanelOpened()
{
	return openedPanel;
}
