#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class MeteoriteTrap : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);

    float Lerp(float a, float b, float t);

    API_Transform centerPointStart;
    int radiusStart;
    API_Vector3 startPos;

    API_Transform centerPointEnd;
    int radiusEnd;
    API_Vector3 endPos;

    float speed = 2.0f;
    float fallingTime;
    float maxFallingTime = 3.0f;

    float cd;
    bool moving = false;

    int minTime = 7;
    int maxTime = 25;

    API_GameObject indicator;
};

