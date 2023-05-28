#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class AidKitRotation : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    float rorationX;
    float rorationY;
    float rorationZ;
};

