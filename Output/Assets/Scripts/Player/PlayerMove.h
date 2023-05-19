#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

#include "PlayerStats.h"

class SwapWeapon;

class PlayerMove : HelloBehavior
{
    enum class PlayerAnims
    {
        IDLE,
        RUN,
        DASH,
        SHOOT,
        SWAP_GUN,
        DEATH,
        OPEN_CHEST,
        JUMPER
    };

public:
    void Start() override;
    void Update() override;

    bool Aim();
    API_Vector2 GetMoveInput();
    float Lerp(float a, float b, float time);
    void DashSetup();
    void Dash();
    bool DashInput();

    void LookAt(API_Vector3 target);

    bool usingGamepad;
    float dt;

    //Movement
    API_Vector2 lastMovInput = { 1.0f, 1.0f };
    API_Vector2 lastAimInput = { 0.0f, 0.0f };
    float vel = 7.5f;
    float upgradedVel = 9.0f;
    float currentVel = 0.0f;
    float departureTime = 0.0f;
    float secToMaxVel = 0.0f;
    float secToZeroVel = 0.0f;
    API_Transform transform;
    API_RigidBody rigidBody;
    float moveSoundCooldown = 0.0f;

    //Void Tp
    API_Vector3 initialPos;
    float yTpLimit = -50.0f;

    //Dash
    bool isDashing = false;
    float dashesAvailable;
    float maxDashCooldown = 6.0f;
    float maxFastDashCooldown = 3.0f;
    float dashCooldown;
    bool dashBuffer;
    float dashDepartTime = 0.0f;
    float dashTime = 0.5f;
    float dashDistance = 6.0f;
    float upgradedDashDistance = 8.0f;

    //Shooting
    float aimAngle;

    // Impulse Player
    void RecieveImpulse(API_Vector3 direction, float impulseDuration, float impulseForce);
    API_Vector3 impulseDirection;
    float impulseStrenght;
    float impulseTime;

    //Animations
    API_AnimationPlayer playerAnimator;
    uint dashAnim = 0;
    uint idle1Anim = 0;
    uint idle2Anim = 0;
    uint idle3Anim = 0;
    bool specialIdleActive = false;
    float specialIdleTime = 1.0f;
    uint runAnim = 0;
    uint shootAnim[7];
    void PlayShootAnim(int gunIndex);
    bool isShooting = false;
    uint swapGunAnim[2];
    void PlaySwapGunAnim(int animationIndex);
    void StopSwapGunAnim();
    bool isSwapingGun = false;
    void PlayIdleAnim();
    uint openChestAnim = 0;
    bool openingChest = false;
    void PlayOpenChestAnim();
    void StopOpenChestAnim();
    uint deathAnim = 0;
    void PlayDeathAnim();
    uint jumperAnim = 0;
    void PlayJumperAnim();
    PlayerAnims currentAnim;

    // particles
    API_ParticleSystem walkParticles;
    bool playingWalkParticles = false;
    API_ParticleSystem shootParticles;

    // skills
    API_GameObject playerStatsGO;
    PlayerStats* playerStats;
};

