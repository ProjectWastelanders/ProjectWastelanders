#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ThanosMovement;
class CamMov;
class PlayerMove;

class ThanosCinematic : HelloBehavior
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
	API_UIImage Dialog_4_1;
	API_UIImage Dialog_5;
	API_UIImage Dialog_6;
	API_UIImage Dialog_7;
	API_UIImage Dialog_8;
	API_UIImage Dialog_9;

	API_GameObject player;
	API_GameObject boss;
	API_GameObject camera;

	API_Vector3 initalPos;
	API_Vector3 finalPos;
	API_Vector3 movingPos;

	ThanosMovement* tMovement;
	CamMov* camMov;
	PlayerMove* playerMov;

	bool activeCinematic, nextDialog, showedDialog;
	int currentDialog;
};

