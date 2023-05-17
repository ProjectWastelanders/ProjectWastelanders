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

    script->AddDragBoxUIImage("Casette_Tutorial_Img", &classInstance->Casette_Img);

    return classInstance;
}

void CasetteManager::Start()
{
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");

    //FeedBack
    initalPos = { -1.250, -0.700, 0 };
    movingPos = { -1.250, -0.700, 0 };
    Casette_Img.GetGameObject().GetTransform().SetPosition(initalPos);
    Casette_Img.GetGameObject().SetActive(false);
    finalPos = { -0.780, -0.700, 0 };

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
    Console::Log("3");
    if ((playerStorage->casette1Picked == true || playerStorage->casette2Picked == true || playerStorage->casette3Picked == true) && CasettePicked == true)
    {
        activeTutorial = true;
        CasettePicked = false;
        Casette_Img.GetGameObject().SetActive(true);
        Console::Log("1");
    }
    FeedBack_Tutorial();
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

void CasetteManager::FeedBack_Tutorial()
{
    if (activeTutorial == true && hideChest == false)
    {
        Casette_Img.GetGameObject().SetActive(true);
        if (Casette_Img.GetGameObject().GetTransform().GetLocalPosition().x < finalPos.x)
        {
            movingPos.x += 0.32 * Time::GetDeltaTime();
            timerTutorial = true;

        }
        if (timerTutorial == true)
        {
            showTutorial += 1.0f * Time::GetDeltaTime();

        }

    }

    if (showTutorial >= 8.0f)
    {

        if (Casette_Img.GetGameObject().GetTransform().GetLocalPosition().x > initalPos.x)
        {
            movingPos.x -= 0.32 * Time::GetDeltaTime();
            timerTutorial = false;
            hideChest = true;
        }

        else if (Casette_Img.GetGameObject().GetTransform().GetLocalPosition().x < initalPos.x) {
            endTutorial = true;
        }



    }
    if (endTutorial == true)
    {
        Casette_Img.GetGameObject().SetActive(false);
    }
    Casette_Img.GetGameObject().GetTransform().SetPosition(movingPos);
}