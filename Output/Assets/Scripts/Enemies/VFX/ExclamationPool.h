#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "ExclamationMark.h"

#include "API/API.h"

class ExclamationPool : HelloBehavior
{
public:
	void Start() override;

	static void ActivateMark(API_GameObject parent);

	API_GameObject mark1;
	API_GameObject mark2;
	API_GameObject mark3;
	API_GameObject mark4;
	API_GameObject mark5;
	API_GameObject mark6;
	API_GameObject mark7;
	API_GameObject mark8;
	API_GameObject mark9;
	API_GameObject mark10;

	static bool active;

private:
	static API_GameObject _marks[10];
	static int _marksCount;
	static int _currentMark;
	bool created = false;
};

