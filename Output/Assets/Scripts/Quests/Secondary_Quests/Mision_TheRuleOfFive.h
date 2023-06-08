#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerStorage;
class EnemyDieEvent;

class Mision_TheRuleOfFive : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintDialog(API_UIImage& Dialog);

	API_GameObject playerStorageGO;
	PlayerStorage* playerStorage;
	EnemyDieEvent* enemyDieEventRef;

	API_UIImage Dialog;

	API_Vector3 initalPos;
	API_Vector3 finalPos;
	API_Vector3 movingPos;

	//int enemiessize = 0;

	//std::vector <API_GameObject> enemies;

	float ruleOfFiveTimer = 0.0f;

	float dialogTimer = 0.0f;
	float _dialogTimer = 0.0f;

	bool misionCompleted = false;

};

