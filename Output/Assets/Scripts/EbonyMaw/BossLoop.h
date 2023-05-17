
#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
class BossLoop : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    void OnCollisionEnter(API::API_RigidBody other);

    void TakeDamage(float damage);

    API_GameObject player;
    API_GameObject boss;
    API_GameObject rockShield;
    API_GameObject cover1;
    API_GameObject cover2;
    API_GameObject cover3;
    API_GameObject cover4;
    API_GameObject cover5;
    API_GameObject cover6;
    API_GameObject cover7;
    API_GameObject cover8;
    API_GameObject cover9;
    API_GameObject cover10;
    API_GameObject cover11;
    API_GameObject cover12;

    //TEMPORAL FOR ALPHA 1
    API_GameObject finalTextPanel;

    float dist;

    float dt;
    float weakTime = 0.0f;

    float time[4] = { 1000, 10.0f,7.5f,6.0f };
    float maxHpLoss[3] = { 1000.0f,500.0f,0.0f };
    float shield[3] = { 500.0f,750.0f,1000.0f };
    float maxShield[3] = { 500.0f,750.0f,1000.0f };

    float hp = 1500.0f;

    bool battle = false;
    bool endBattle = false;

    bool isBossAlive = false;
    bool isBossActive = false;
    bool canTakeDamage = false;
    bool exploting = false;
    bool readyToExplote = false;
    int phase = 0;

    float meleeDmg = 10.0f;

    float maxResistance = 100.0f;
    float currentResistance;
    float speed = 75.0f;
    float acceleration = 1.2f;
    float currentSpeed = 0.0f;

    // bomb mechanic
    void AddBomb();
    void CheckBombs();
    int maxBombNum = 6;
    int currentBombNum = 0;
    API_GameObject bomb;
    API_GameObject bombShield;
    uint textureBomb[6];
    int shotgunLevel;

    // burn
    void AddBurn();
    float burnTime = 0.0f;
    float resetBurn;


    float DieTimer;
    float recoverTimer;
    float knockUpTimer;

    enum class AnimationState
    {
        NONE,
        IDLE,
        IDLE2,
        KNOCKUP,
        MOVE,
        MOVEOBJ,
        RECOVER,
        SPECIAL,
        SPECIAL2,
        TAKEOBJ,
        THROWOBJ,
        DIE
    };

    AnimationState animState = AnimationState::IDLE;

    API_AnimationPlayer animationPlayer;
    uint idleAnim;
    uint idleAnim2;
    uint knockUpAnim;
    uint movingAnim;
    uint movingWithObjAnim;
    uint recoverAnim;
    uint specialAnim;
    uint specialAnim2;
    uint takeObjAnim;
    uint throwObjAnim;
    uint dieAnim;
};
