#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class Weapon_Select : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	API_UIButton weapon1;
	API_UIButton weapon2;
	API_UIButton weapon3;
	API_UIButton weapon4;

	API_UIImage infoWeapon;
	API_UIImage weaponImage;

	API_UIInput weaponSelectPanel;
	API_UIInput proceedPanel;

	uint info1;
	uint info2;
	uint info3;
	uint info4;

	uint weaponImage1;
	uint weaponImage2;
	uint weaponImage3;
	uint weaponImage4;

	bool isPress;
	bool inOpen;
	int indexLevles;

};

