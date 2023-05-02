#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Jumper : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	float Lerp(float a, float b, float t);

	//Config
	API_GameObject* player;
	API_GameObject* otherJumper;
	float duration = 4.0f;
	
	bool validScript = true;
	
	//Jumping & Setup
	bool isJumping = false;
	float timeJumping;
	API_Vector3 initialPos;
	API_Vector3 finalPos;
	void SetupJump();
};

