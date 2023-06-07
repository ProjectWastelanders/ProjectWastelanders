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

    void OnCollisionStay(API_RigidBody other);
    void OnCollisionEnter(API_RigidBody other);

    void ShootWall(float projectileDamage);
    void DestroyWall();
    void PopUpTutorial(bool active);

    API_ParticleSystem wallDestroyed;

    API_GameObject fenceDestroyed;
    API_GameObject fenceEntire;

    API_RigidBody fenceRigidbody;

    API_UIImage Tutorial_Img;

    API_Vector3 initalPos;
    API_Vector3 finalPos;
    API_Vector3 movingPos;

    int maxHp;
    int currentHp;

    bool tutorial_active = false;
    bool timerTutorial = false;
    bool hideChest = false;
    float showTutorial = 0;

    bool isDestroyed;

    API_AnimationPlayer animationPlayer1;
    API_AnimationPlayer animationPlayer2;
    API_AnimationPlayer animationPlayer3;

    uint breakAnim;

};

