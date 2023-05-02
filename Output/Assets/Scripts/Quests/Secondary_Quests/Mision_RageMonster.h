#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class Mision_RageMonster : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject boxes[16];

    bool misionCompleted = false;

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;
};

