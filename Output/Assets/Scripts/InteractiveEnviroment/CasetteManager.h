#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;
class MusicManager;
class PlayerStats;

class CasetteManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;
    void FeedBack_Tutorial();
    void ActiveDialogs();
    void PrintDialog(API_UIImage& Dialog);

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
    PlayerStats* playerStats = nullptr;

    //FeedBack Tutorial
    API_UIImage Casette_Img;

    API_Vector3 initalPos;
    API_Vector3 finalPos;
    API_Vector3 movingPos;

    bool activeTutorial = false;
    bool endTutorial = false;
    bool timerTutorial = false;
    bool hideChest = false;

    float showTutorial = 0;

    bool playedCasetteMusic[3] = { false,false,false };
};