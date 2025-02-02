#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ExclamationMark : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void OnEnable() override;
	void OnDisable() override;

	API_GameObject _pull;

private:
	float _count = 2.0f;
};

