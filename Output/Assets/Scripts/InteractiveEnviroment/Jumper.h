#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerMove;

class Jumper : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API_RigidBody other);
	float Lerp(float a, float b, float t);

	//Config
	API_GameObject player;
	PlayerMove* playerMoveScript;
	API_Transform otherJumper;
	float duration = 4.0f;
	float height = 50.0f;
	
	//Jumping & Setup
	bool Inactive = false;
	bool isJumping = false;
	float timeJumping;
	API_Vector3 initialPos;
	API_Vector3 finalPos;
	void SetupJump();
};

