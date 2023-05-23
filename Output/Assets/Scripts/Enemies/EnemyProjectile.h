#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class EnemyProjectile : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void Destroy();

    void OnCollisionEnter(API::API_RigidBody other) override;

    API_RigidBody rigidbody;

    float speed = 2.0f;
    float lifeTime = 5.0f;
    float damage = 0.0f;
    float resistanceDamage = 0.0f;
};

