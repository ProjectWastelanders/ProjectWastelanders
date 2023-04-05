#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class BreakableWall : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);

    void ShootWall(float projectileDamage);
    void DestroyWall();

    API_ParticleSystem wallDestroyed;

    int maxHp;
    int currentHp;
};

