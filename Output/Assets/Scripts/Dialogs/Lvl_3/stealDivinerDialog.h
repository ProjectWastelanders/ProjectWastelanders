#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class stealDivinerDialog : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintDialog(API_UIImage& Dialog);

    API_UIImage Dialog_1;
    API_UIImage Dialog_2;
    API_UIImage Dialog_3;
    API_UIImage Dialog_4;
    API_UIImage Dialog_5;
    API_UIImage Dialog_6;
    API_UIImage Dialog_7;
    API_UIImage Dialog_8;
    API_UIImage Dialog_9;
    API_UIImage Dialog_10;
    API_UIImage Dialog_11;
    API_UIImage Dialog_12;

    API_Vector3 initalPos;
    API_Vector3 finalPos;
    API_Vector3 movingPos;

    bool activeDialogs;
    int currentDialog;

    float timer = 0.0f;
    float _timer = 0.0f;
};

