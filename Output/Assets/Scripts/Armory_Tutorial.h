#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Armory_Tutorial : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;
	void PrintTutorial(API_GameObject &tutorial);

	API_GameObject armoryScreen;
	API_GameObject tutorial1;
	API_GameObject tutorial2;
	API_GameObject itsMe;

	bool activeTutorial;
	int currentTutorial;
	/*bool tutorial1_active;
	bool tutorial2_active;*/

	Armory_Tutorial* _script;
};

