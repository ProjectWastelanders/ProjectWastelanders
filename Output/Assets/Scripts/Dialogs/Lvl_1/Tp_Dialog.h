#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStats;

class Tp_Dialog : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintDialog(API_UIImage& Dialog, string audioEvent);

	API_GameObject playerGO;
	PlayerStats* playerStats;

	API_UIImage Dialog_1;

	API_Vector3 initalPosDialog;
	API_Vector3 finalPosDialog;
	API_Vector3 movingPosDialog;
	float timer = 0.0f;
	float _timer = 0.0f;
	bool activeAudio;
};

