#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class StickyBombParticle : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    float time;
};

