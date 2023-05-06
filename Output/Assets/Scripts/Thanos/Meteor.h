#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Meteor : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnCollisionEnter(API::API_RigidBody other);

	API_Vector3 position;
	API_ParticleSystem fire;
	bool isFireOn = false;
	float fireCounter = 0.0f;

	float dmg = 5.0f;
	bool hasCollided = false;
};

