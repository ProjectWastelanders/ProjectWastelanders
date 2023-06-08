#include "ReachTheSpaceship.h"
#include "../Player/PlayerStorage.h"
HELLO_ENGINE_API_C ReachTheSpaceship* CreateReachTheSpaceship(ScriptToInspectorInterface* script)
{
    ReachTheSpaceship* classInstance = new ReachTheSpaceship();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxGameObject("Player Storage GO", &classInstance->playerStorageGO);
    script->AddDragBoxGameObject("Final Text Panel", &classInstance->finalText);
    script->AddDragFloat("Max HP", &classInstance->maxHp);
    script->AddDragFloat("Current HP", &classInstance->currentHp);
    script->AddDragFloat("DamagePerProjectile", &classInstance->damagePerProjectile);
    script->AddDragBoxGameObject("Enemy to Active 0", &classInstance->enebledEnemies[0]);
    script->AddDragBoxGameObject("Enemy to Active 1", &classInstance->enebledEnemies[1]);
    script->AddDragBoxGameObject("Enemy to Active 2", &classInstance->enebledEnemies[2]);
    script->AddDragBoxGameObject("Enemy to Active 3", &classInstance->enebledEnemies[3]);
   /* script->AddDragBoxGameObject("Enemy to Active 4", &classInstance->enebledEnemies[4]);
    script->AddDragBoxGameObject("Enemy to Active 5", &classInstance->enebledEnemies[5]);
    script->AddDragBoxGameObject("Enemy to Active 6", &classInstance->enebledEnemies[6]);
    script->AddDragBoxGameObject("Enemy to Active 7", &classInstance->enebledEnemies[7]);*/
    script->AddCheckBox("aaaaaaaaaaaaaaaa", &classInstance->enabled);

    return classInstance;
}

void ReachTheSpaceship::Start()
{
    currentHp = maxHp;
    playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
    if (!playerStorage) Console::Log("Storage Missing in ReachSpaceShip Script.");


    for (size_t i = 0; i < 4; i++)
    {
        spawnPosShip[i].x = enebledEnemies[i].GetTransform().GetGlobalPosition().x;
        spawnPosShip[i].y = enebledEnemies[i].GetTransform().GetGlobalPosition().y;
        spawnPosShip[i].z = enebledEnemies[i].GetTransform().GetGlobalPosition().z;
    }

    for (size_t i = 0; i < 4; i++)
    {
        enebledEnemies[i].GetTransform().SetPosition(10000, 0, 10000);
    }
    
    //for (size_t i = 0; i < 3; i++)
    //{
    //    //enebledEnemies[i].SetActive(true);
    //    //enebledEnemies[i].GetTransform().SetPosition();
    //    enemyScript[i] = (Enemy*)enebledEnemies[i].GetScript("Enemy");
    //    if(!enemyScript[i]) Console::Log("Obamaaaaaa");
    //}

    /*enemyScript1 = (Enemy*)enebledEnemies[0].GetScript("Enemy");
    if (!enemyScript1) Console::Log("Enemy Missing in ReachSpaceShip Script.");

    enemyScript2 = (Enemy*)enebledEnemies[1].GetScript("Enemy");
    if (!enemyScript2) Console::Log("Enemy Missing in ReachSpaceShip Script.");

    enemyScript3 = (Enemy*)enebledEnemies[2].GetScript("Enemy");
    if (!enemyScript3) Console::Log("Enemy Missing in ReachSpaceShip Script.");

    enemyScript4 = (Enemy*)enebledEnemies[3].GetScript("Enemy");
    if (!enemyScript4) Console::Log("Enemy Missing in ReachSpaceShip Script.");*/
    //EnableMision();
}

void ReachTheSpaceship::Update()
{
    //Console::Log(std::to_string(currentHp));
    

    //if (Input::GetKey(KeyCode::KEY_J) == KeyState::KEY_DOWN) currentHp -= 100;
    //if (currentHp <= 0.0f)
    //{
    //    //Mision Failed
    //    Scene::LoadScene("LoseMenu.HScene");
    //}
   /* for (size_t i = 0; i < 8; i++)
    {
        enebledEnemies[i].SetActive(false);
    }*/

   /* if (enabled)
    {
        cooldownHit += Time::GetDeltaTime();
        if (cooldownHit >= timeHit)
        {
            currentHp -= damagePerProjectile*8;
            cooldownHit = 0;
        }
    }*/
}

void ReachTheSpaceship::OnCollisionEnter(API_RigidBody other)
{
    if (!enabled) return;
   //if(( Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN) currentHp -=100;

      // if (Input::GetKey(KeyCode::KEY_J) == KeyState::KEY_DOWN) currentHp -= 100;
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "EnemyProjectile")
    {
        currentHp -= damagePerProjectile;

       //other.GetGameObject().

        if (currentHp <= 0.0f)
        {
            //Mision Failed
            Scene::LoadScene("LoseMenu.HScene");
        }

    }
    else if (detectionTag == "Player")
    {
        if (playerStorage)
        {
            playerStorage->skillPoints += 3;
            if (currentHp >= maxHp / 2.0f) // secondary mision
            {
                playerStorage->skillPoints += 2;
            }
            playerStorage->SaveData();
        }
        API_QuickSave::SetBool("level3_completed", true);
        API_QuickSave::SetBool("IsInMiddleOfLevel", false);
        Scene::LoadScene("SpaceshipHUB_Scene.HScene");
        uwu = true;
        finalText.SetActive(true);
    }
}

void ReachTheSpaceship::EnableMision()
{
    enabled = true;

    for (size_t i = 0; i < 4; i++)
    {
        //enebledEnemies[i].SetActive(true);
    
       // Console::Log("Obamaaaaaa");
            enebledEnemies[i].GetTransform().SetPosition(spawnPosShip[i]);
        //if (enemyScript[i])
        //{
        //    Console::Log("Obamaaaaaa");
        //  //  enebledEnemies[i].GetTransform().SetPosition((enemyScript[i]->spawnPos.x, enemyScript[i]->spawnPos.y, enemyScript[i]->spawnPos.z));
        //}
    }
}