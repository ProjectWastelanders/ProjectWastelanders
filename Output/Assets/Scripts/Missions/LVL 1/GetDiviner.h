#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;

class GetDiviner : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionStay(API_RigidBody other);

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;
    API_GameObject finalText;
};

