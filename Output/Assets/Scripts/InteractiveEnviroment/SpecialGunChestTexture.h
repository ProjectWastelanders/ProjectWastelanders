#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Chest;

class SpecialGunChestTexture : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_GameObject chestLogicReference;
    Chest* chestScript;
    API_MeshRenderer render;
};

