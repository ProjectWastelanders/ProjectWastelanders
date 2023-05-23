#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class ExclamationMarkPool : HelloBehavior
{
public:
	void Start() override; 

	void ActivateMark(API_GameObject parent);

private:
	API_GameObject* _marks;
	int _marksCount = 0;
	int _currentMark = 0;
};

