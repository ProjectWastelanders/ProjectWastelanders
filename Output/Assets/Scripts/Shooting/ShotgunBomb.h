#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ShotgunBombExplosion;

class ShotgunBomb : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other);

    void Destroy();
    void ResetExposion();

    float speed = 10.0f;
    float lifeTime = 5.0f;

    ShotgunBombExplosion* explosion;
    uint ignoreGO = 0;
};

