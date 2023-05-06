#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SmokeTrapProjectile : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

    void Destroy();

    void OnCollisionEnter(API::API_RigidBody other);

    float speed = 2.0f;
    float lifeTime = 5.0f;
};

