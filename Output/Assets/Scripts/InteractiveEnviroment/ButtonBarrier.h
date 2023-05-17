#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ButtonBarrier : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionStay(API::API_RigidBody other) override;
    void OnCollisionEnter(API::API_RigidBody other) override;
    void OnCollisionExit(API::API_RigidBody other) override;

    bool picked = false;
    bool isOver = false;
    float openBarrierTime;
    float maxOpenBarrierTime = 1.0f;
    

    //Button
    API_UIImage HoldBlue;
    API_UIImage HoldText;
    API_UIImage HoldBlack;
    API_UIImage HoldButton;

    float openBarrierTimeBar;
    

};

