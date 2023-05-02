#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ButtonBarrier;

class BarrierWithButton : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject buttonGO;
    ButtonBarrier* button;

    API_GameObject lamp1;
    API_GameObject lamp2;
    API_RigidBody barrierRb;

    uint materialLamps;
    uint materialLamp1;
    uint materialLamp2;

    bool opened = false;
};

