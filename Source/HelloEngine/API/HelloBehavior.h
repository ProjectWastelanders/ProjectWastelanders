#pragma once
#include "Globals.h"
#include "API/API_GameObject.h"
#include "API/API_RigidBody.h"

class TO_API HelloBehavior
{
public:
	HelloBehavior() {};
	virtual ~HelloBehavior() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};

	virtual void OnEnable() {};
	virtual void OnDisable() {};

	// TODO: Create an API_PhysBody that can be accesed thorugh scripting and have that as an argument of OnCollision methods.
	virtual void OnCollisionEnter(API::API_RigidBody other) {};
	virtual void OnCollisionStay(API::API_RigidBody other) {};
	virtual void OnCollisionExit(API::API_RigidBody other) {};
public:
	API::API_GameObject gameObject;

	friend class LayerGame;
};

