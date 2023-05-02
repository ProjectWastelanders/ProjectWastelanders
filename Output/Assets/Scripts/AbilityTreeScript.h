#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"

#include "API/API.h"
#include "API/API_UIInput.h"

class OpenMenuInterruptor;
class PlayerMove;

class AbilityTreeScript : HelloBehavior
{
public:
    void Start() override;
    void Update() override;

    API_UIInput mainPanel;

    API_UIButton AbilityList1;
    API_UIButton AbilityList2;
    API_UIButton AbilityList3;
    API_UIButton AbilityList4;

    API_GameObject PanelAbility1;
    API_GameObject PanelAbility2;
    API_GameObject PanelAbility3;
    API_GameObject PanelAbility4;

    API_GameObject interruptorGO;
    OpenMenuInterruptor* interruptor;

    API_GameObject playerGO;
    PlayerMove* playerMove;

    bool ability1 = true;
    bool ability2 = true;
    bool ability3 = true;
    bool ability4 = true;
};
