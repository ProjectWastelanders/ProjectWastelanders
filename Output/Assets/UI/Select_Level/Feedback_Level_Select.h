#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIText.h"

class OpenMenuInterruptor;
class PlayerMove;

class Feedback_Level_Select : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	//void OnCollisionStay(API::API_RigidBody other) override;
	void OnCollisionEnter(API::API_RigidBody other) override;
	void OnCollisionExit(API::API_RigidBody other) override;

	void OnCharge();
	void SetCurrentLevel();
	void LevelSelect();

	API_GameObject checkMainMision1;
	API_GameObject checkSecundaryMision1;
	API_GameObject checkSecundaryMision2;
	API_GameObject interruptorGO;

	API_GameObject mapImage;
	API_GameObject planetImage;
	API_GameObject planet_Name;

	API_UIImage chargingBar;
	API_UIImage isSelected;

	API_UIImage mainMision;
	API_UIImage secMision1;
	API_UIImage secMision2;

	API_GameObject panel;
	API_UIInput chargingConjunt;

	API_UICheckBox level1;
	API_UICheckBox level2;
	API_UICheckBox level3;
	API_UICheckBox level4;
	API_UICheckBox currentLevel;

	OpenMenuInterruptor* interruptor;

	API_GameObject playerGO;
	PlayerMove* playerMove;

	API_UIText casettesNumberText;
	int casettesNumber;

	uint mainMision1;
	uint mainMision2;
	uint mainMision3;
	uint mainMision4;

	uint secundaryMision1_1;
	uint secundaryMision2_1;
	uint secundaryMision1_2;
	uint secundaryMision2_2;
	uint secundaryMision1_3;
	uint secundaryMision2_3;
	uint secundaryMision1_4;
	uint secundaryMision2_4;

	uint map1;
	uint map2;
	uint map3;
	uint map4;

	uint planet1;
	uint planet2;
	uint planet3;
	uint planet4;

	uint planetName1;
	uint planetName2;
	uint planetName3;
	uint planetName4;

	int indexLevles;

	float openSelecTime;
	float maxOpenSelectTime = 1.0f;

	bool selecting;
	bool isPress;
};

