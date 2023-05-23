#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ExclamationMarkPull : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

private:
	std::vector<API_GameObject> marks;
};

