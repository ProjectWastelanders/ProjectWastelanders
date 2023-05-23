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

	virtual void OnCollisionEnter(API::API_RigidBody other) {};
	virtual void OnCollisionStay(API::API_RigidBody other) {};
	virtual void OnCollisionExit(API::API_RigidBody other) {};
public:
	API::API_GameObject gameObject;

	friend class LayerGame;
};

