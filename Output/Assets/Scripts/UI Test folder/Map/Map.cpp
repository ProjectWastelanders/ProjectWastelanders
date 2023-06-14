#include "Map.h"
#include "../../Player/PlayerStorage.h"
HELLO_ENGINE_API_C Map* CreateMap(ScriptToInspectorInterface* script)
{
    Map* classInstance = new Map();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Casette1", &classInstance->casette1);
    script->AddDragBoxGameObject("Casette2", &classInstance->casette2);
    script->AddDragBoxGameObject("Casette3", &classInstance->casette3);
    script->AddDragInt("Level Index", &classInstance->levelIndex);
    script->AddDragBoxGameObject("Map GO", &classInstance->mapGO);
    return classInstance;
}

void Map::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in CasetteManager.");
    if (levelIndex == 2)
    {
        mapGO.SetActive(false);
    }
    else
    {
        mapGO.SetActive(true);
    }

}

void Map::Update()
{
    /*if (Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_M) == KeyState::KEY_DOWN)
    {
        if (mapOpen)
        {
            mapGO.SetActive(false);
            mapOpen = false;
        }
        else
        {
            mapGO.SetActive(true);
            mapOpen = true;

            if (playerStorage == nullptr) return;
            casette1.SetActive(playerStorage->casette1Picked);
            casette2.SetActive(playerStorage->casette2Picked);
            casette3.SetActive(playerStorage->casette3Picked);
        }
    }*/
}

void Map::MissionsEnable(bool hasMap)
{
    if (true)
    {
        if (playerStorage == nullptr) return;
        casette1.SetActive(playerStorage->casette1Picked);
        casette2.SetActive(playerStorage->casette2Picked);
        casette3.SetActive(playerStorage->casette3Picked);
    }
    else
    {
        
        casette1.SetActive(false);
        casette2.SetActive(false);
        casette3.SetActive(false);
    }

}
