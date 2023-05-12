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

    bool playedCasetteMusic[3] = { false,false,false };
};

