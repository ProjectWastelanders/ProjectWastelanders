#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SmokeExtractorTrap : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API_RigidBody other);
    void OnCollisionExit(API_RigidBody other);

    API_GameObject GetFirstInactiveProjectile();

    API_ParticleSystem smoke;
    API_ParticleSystem fire;
    API_GameObject playerRef;

    API_Vector3 goPos;

    int minDistance = 5;

    float delay;
    float currentDelay = 0.0f;
    float fireTimer;
    float fireSeconds;

    bool throwFire = false;
    bool hitPlayer = true;
    bool playerOnRange = false;

    int pullSize;
    std::vector<API_GameObject> pull;
    uint projectilePrefab;
    float lifeTime = 0;
    float speed = 0;

    float maxProjectileDelay = 0.2f;
    float projectileDelay = 0;
};

