#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BoxManager : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    int boxesAmount;
    API_GameObject boxes[99];

    void SaveBoxesState(int levelIndex);
    void LoadBoxesState(int levelIndex);
};

