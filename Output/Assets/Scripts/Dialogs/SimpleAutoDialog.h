#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class SimpleAutoDialog : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintDialog(API_UIImage& Dialog);
	void OnCollisionEnter(API::API_RigidBody other) override;

	API_UIImage Dialog;

	API_Vector3 initalPos;
	API_Vector3 finalPos;
	API_Vector3 movingPos;

	API_String audioEvent;

	bool activeDialog;

	float timer = 0.0f;
	float _timer = 0.0f;
};

