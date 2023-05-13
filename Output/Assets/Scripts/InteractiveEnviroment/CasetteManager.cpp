#include "CasetteManager.h"
#include "../Player/PlayerStorage.h"
#include "../Audio/MusicManager.h"

HELLO_ENGINE_API_C CasetteManager* CreateCasetteManager(ScriptToInspectorInterface* script)
{
    CasetteManager* classInstance = new CasetteManager();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Casette 1", &classInstance->casette1);
    script->AddDragBoxGameObject("Casette2", &classInstance->casette2);
    script->AddDragBoxGameObject("Casette 3", &classInstance->casette3);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddInputBox("Audio Event String 1", &classInstance->playAudio1);
    script->AddInputBox("Audio Event String 2", &classInstance->playAudio2);
    script->AddInputBox("Audio Event String 3", &classInstance->playAudio3);
    return classInstance;
}

void CasetteManager::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (playerStorage == nullptr) Console::Log("PlayerStorage missing in CasetteManager.");
    else
    {
        //playerStorage->playAudio1 = playAudio1;
        //playerStorage->playAudio2 = playAudio2;
        //playerStorage->playAudio3 = playAudio3;
    }

    musicManager = (MusicManager*)Game::FindGameObject("MusicManager").GetScript("MusicManager");
    if (musicManager == nullptr) Console::Log("Music manager missing in CasetteManager");
    check = false;
}

void CasetteManager::Update()
{
    if (!playerStorage) return;
    if (playerStorage->casette1Picked && !playedCasetteMusic[0]) 
    {
        casette1.SetActive(false);
        musicManager->PlayCasette(0);
        playedCasetteMusic[0] = true;
    }
    if (playerStorage->casette2Picked && !playedCasetteMusic[1])
    {
        casette2.SetActive(false);
        musicManager->PlayCasette(1);
        playedCasetteMusic[1] = true;
    }
    if (playerStorage->casette3Picked && !playedCasetteMusic[2])
    {
        casette3.SetActive(false);
        musicManager->PlayCasette(2);
        playedCasetteMusic[2] = true;
    }
    check = true;
}