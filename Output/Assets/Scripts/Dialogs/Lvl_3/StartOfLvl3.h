#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class StartOfLvl3 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintDialog(API_UIImage& Dialog);
	void OnCollisionEnter(API::API_RigidBody other) override;

	API_UIImage Dialog_1;
	API_UIImage Dialog_2;
	API_UIImage Dialog_3;
	API_UIImage Dialog_4;

	API_Vector3 initalPos;
	API_Vector3 finalPos;
	API_Vector3 movingPos;

	bool activeDialogs, nextDialog, activeAudio;
	int currentDialog;

	float timer = 0.0f;
	float _timer = 0.0f;
};

