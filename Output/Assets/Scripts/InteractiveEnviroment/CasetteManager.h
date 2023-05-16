#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;
class MusicManager;

class CasetteManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;
    void FeedBack_Tutorial();

    API_GameObject casette1;
    API_GameObject casette2;
    API_GameObject casette3;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage = nullptr;

    bool check;
    API_String playAudio1;
    API_String playAudio2;
    API_String playAudio3;
    MusicManager* musicManager = nullptr;

    //FeedBack Tutorial
    API_UIImage Casette_Img;

    API_Vector3 initalPos;
    API_Vector3 finalPos;
    API_Vector3 movingPos;

    bool activeTutorial = false;
    bool endTutorial = false;
    bool timerTutorial = false;
    bool hideChest = false;
    bool CasettePicked = true;

    float showTutorial = 0;

    bool playedCasetteMusic[3] = { false,false,false };
};

