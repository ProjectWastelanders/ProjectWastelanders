#pragma once
#include "API/HelloBehavior.h"
#include "ScriptToInspectorInterface.h"
#include "Macro.h"
#include "../CamMov.h"
#include "API/API.h"

class PlayerStorage;
class ReachTheSpaceship;
class stealDivinerDialog;

class StealTheDivinerAgain : HelloBehavior
{
public:
    void Start() override;
    void Update() override;
    void OnCollisionStay(API_RigidBody other);

    API_GameObject playerStorageGO;
    PlayerStorage* playerStorage;
    API_GameObject finalText;

    API_GameObject Dialog;
    stealDivinerDialog* stealDiviner_Dialog;

    API_GameObject reachTheSpaceShipGO;
    ReachTheSpaceship* reachTheSpaceShip;

    CamMov* cam;
    API_GameObject camObj;
    bool uwu = false;
};

