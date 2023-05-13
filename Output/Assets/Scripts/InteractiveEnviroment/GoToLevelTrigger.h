#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class GoToLevelTrigger : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);

    API_GameObject panel;

    /*API_String scene = "";
    int nextLevel;
    bool isHub = false;*/
};

