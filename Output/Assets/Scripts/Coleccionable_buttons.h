#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../Scripts/Player/PlayerStorage.h"
#include "API/API.h"

class OpenMenuInterruptor;
class PlayerMove;

class Coleccionable_buttons : HelloBehavior
{
public:
	void Start() override; 
	void Update() override;

	void Level1selected( bool casete1, bool casete2,bool casete3);
	void Level2selected( bool casete1, bool casete2,bool casete3);
	void Level3selected( bool casete1, bool casete2,bool casete3);

	API_UICheckBox checkBox[3];

	API_GameObject coleccionable_panel;

	API_UIImage level_selected;

	uint levels[3];
	
	API_GameObject casetes[3];

	API_GameObject playerStorageGO;
	PlayerStorage* playerStorage;
	PlayerMove* playermovment;

	API_GameObject interruptor_Colec_GO;
	OpenMenuInterruptor* interruptor_Colec;

	uint song1 = 1;
	uint song2 = 2;
	uint song3 = 3;

};

