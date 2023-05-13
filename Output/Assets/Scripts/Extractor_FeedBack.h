#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Extractor_FeedBack : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
    void OnCollisionEnter(API::API_RigidBody other) override;
    void OnCollisionExit(API::API_RigidBody other) override;

    API_UIImage Extractor_Img;

    API_Vector3 initalPos;
    API_Vector3 finalPos;
    API_Vector3 movingPos;

    bool activeTutorial = false;
    bool endTutorial = false;
    bool timerTutorial = false;
    bool hideChest = false;

    float showTutorial = 0;
};

