#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "BossLoop.h"
#include "BossAttacks.h"
#include "../CamMov.h"
#include "../Player/PlayerMove.h"

#include "API/API.h"

class BossCinematic : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintDialog(API_UIImage &Dialog);
	void OnCollisionEnter(API::API_RigidBody other) override;

	API_UIImage Dialog_1;
	API_UIImage Dialog_2;
	API_UIImage Dialog_3;
	API_UIImage Dialog_4;
	API_UIImage Dialog_5;
	API_UIImage Dialog_6;
	API_UIImage Dialog_StartBattle;
	API_UIImage Dialog_EndBattle;

	API_GameObject player;
	API_GameObject boss;
	API_GameObject camera;

	API_Vector3 initalPos;
	API_Vector3 finalPos;
	API_Vector3 movingPos;
	
	BossLoop* bLoop;
	BossAttacks* bAttacks;
	CamMov* camMov;
	PlayerMove* playerMov;

	bool DraxAudio, endBattleAudio;

	bool activeCinematic, nextDialog, animBoss, activeAudio;
	bool showedCinematic, showedStartDialog;
	int currentDialog;
	float timerSB = 0.0f;
	float _timerSB = 0.0f;
	float timerEB = 0.0f;
	float _timerEB = 0.0f;

};


