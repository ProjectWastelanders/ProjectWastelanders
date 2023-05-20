#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class KillEnemiesZone2 : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void CheckEnemies();
	void PrintDialog(API_UIImage& Dialog);

	API_GameObject enemy_1;
	API_GameObject enemy_2;
	API_GameObject enemy_3;
	API_GameObject enemy_4;
	API_GameObject enemy_5;
	API_GameObject enemy_6;
	API_GameObject enemy_7;
	API_GameObject enemy_8;
	API_GameObject enemy_9;
	API_GameObject enemy_10;
	API_GameObject enemy_11;

	int EnemyCount = 0;

	API_UIImage Dialog;

	API_Vector3 initalPos;
	API_Vector3 finalPos;
	API_Vector3 movingPos;

	bool printDialog, printedDialog;

	float timer = 0.0f;
	float _timer = 0.0f;
};

