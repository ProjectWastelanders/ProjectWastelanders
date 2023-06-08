#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"

class PlayerMove;

class HUB_UIManager : HelloBehavior
{
public:
	void Start() override; 

	static void OpenPanel();
	static void ClosePanel();
	static bool IsPanelOpened();

	static bool openedPanel;
	static PlayerMove* player;
};

