#include "KillEnemiesZone2.h"
HELLO_ENGINE_API_C KillEnemiesZone2* CreateKillEnemiesZone2(ScriptToInspectorInterface* script)
{
	KillEnemiesZone2* classInstance = new KillEnemiesZone2();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    /*script->AddDragBoxGameObject("Enemy 1", &classInstance->enemy_1);
    script->AddDragBoxGameObject("Enemy 2", &classInstance->enemy_2);
    script->AddDragBoxGameObject("Enemy 3", &classInstance->enemy_3);
    script->AddDragBoxGameObject("Enemy 4", &classInstance->enemy_4);
    script->AddDragBoxGameObject("Enemy 5", &classInstance->enemy_5);
    script->AddDragBoxGameObject("Enemy 6", &classInstance->enemy_6);
    script->AddDragBoxGameObject("Enemy 7", &classInstance->enemy_7);
    script->AddDragBoxGameObject("Enemy 8", &classInstance->enemy_8);
    script->AddDragBoxGameObject("Enemy 9", &classInstance->enemy_9);
    script->AddDragBoxGameObject("Enemy 10", &classInstance->enemy_10);
    script->AddDragBoxGameObject("Enemy 11", &classInstance->enemy_11);*/

    script->AddDragBoxUIImage("Dialog", &classInstance->Dialog);

    script->AddDragFloat("Dialog timer", &classInstance->timer);

    script->AddDragFloat("Dialog Pos X", &classInstance->finalPos.x);
    script->AddDragFloat("Dialog Pos Y", &classInstance->finalPos.y);
    script->AddDragFloat("Dialog Pos Z", &classInstance->finalPos.z);
	return classInstance;
}

void KillEnemiesZone2::Start()
{
    //Game::FindGameObjectsWithTag("EnemiesZone2", enemies[0], 11);

    initalPos = { 0, -1.500, 0 };
    movingPos = { 0, -1.500, 0 };
    //finalPos = { 0, -0.500, 0 };

    printDialog = false; 
    printedDialog = false;

    Dialog.GetGameObject().GetTransform().SetPosition(initalPos);
    Dialog.GetGameObject().SetActive(false);
}
void KillEnemiesZone2::Update()
{
    //CheckEnemies();
    if (EnemyCount == 0) {
        printDialog = true;
    }

    if (printDialog && !printedDialog) {
        PrintDialog(Dialog);
    }
}

//void KillEnemiesZone2::CheckEnemies()
//{
//    if (enemy_1.IsAlive() == false && enemy_1.IsAlive() == false && enemy_1.IsAlive() == false && enemy_1.IsAlive() == false &&
//        enemy_1.IsAlive() == false && enemy_1.IsAlive() == false && enemy_1.IsAlive() == false && enemy_1.IsAlive() == false &&
//        enemy_1.IsAlive() == false && enemy_1.IsAlive() == false && enemy_1.IsAlive() == false)
//    {
//        printDialog = true;
//    }
//}

void KillEnemiesZone2::PrintDialog(API_UIImage& Dialog)
{
    Dialog.GetGameObject().SetActive(true);

    if (_timer >= timer) {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y > initalPos.y)
        {
            movingPos.y -= 1 * Time::GetDeltaTime();
        }
        else {
            printedDialog = true;
            Dialog.GetGameObject().SetActive(false);
        }
    }
    else {
        if (Dialog.GetGameObject().GetTransform().GetGlobalPosition().y < finalPos.y)
        {
            movingPos.y += 1 * Time::GetDeltaTime();
        }
        else
        {
            _timer += Time::GetDeltaTime();
        }
    }

    Dialog.GetGameObject().GetTransform().SetPosition(movingPos);
}

void KillEnemiesZone2::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Enemy")
    {
        //printDialog = true;
        EnemyCount++;
    }
}