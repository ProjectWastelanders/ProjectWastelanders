#include "MeteoriteTrap.h"
#include "../Player/PlayerStats.h"
HELLO_ENGINE_API_C MeteoriteTrap* CreateMeteoriteTrap(ScriptToInspectorInterface* script)
{
    MeteoriteTrap* classInstance = new MeteoriteTrap();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragBoxTransform("Initial Center Point", &classInstance->centerPointStart);
    script->AddDragInt("Initial Radius", &classInstance->radiusStart);
    script->AddDragBoxTransform("Final Center Point", &classInstance->centerPointEnd);
    script->AddDragInt("Final Radius", &classInstance->radiusEnd);
    script->AddDragFloat("Speed", &classInstance->speed);
    script->AddDragFloat("Falling Time", &classInstance->maxFallingTime);
    script->AddDragInt("Min Time", &classInstance->minTime);
    script->AddDragInt("Max Time", &classInstance->maxTime);
    script->AddDragBoxGameObject("Indicator", &classInstance->indicator);
    return classInstance;
}

void MeteoriteTrap::Start()
{
    cd = rand() % (maxTime - minTime) + minTime;
}

void MeteoriteTrap::Update()
{
    if (!moving)
    {
        cd -= Time::GetDeltaTime();
        if (cd <= 0.0f)
        {
            cd = 0.0f;
            moving = true;

            fallingTime = 0;
            
            // randomize position X and Z
            int randX = rand() % (radiusStart * 2) - radiusStart;
            int randZ = rand() % (radiusStart * 2) - radiusStart;
            startPos = { centerPointStart.GetGlobalPosition().x + randX, centerPointStart.GetGlobalPosition().y, centerPointStart.GetGlobalPosition().z + randZ };
            randX = rand() % (radiusEnd * 2) - radiusEnd;
            randZ = rand() % (radiusEnd * 2) - radiusEnd;
            endPos = { centerPointEnd.GetGlobalPosition().x + randX, centerPointEnd.GetGlobalPosition().y, centerPointEnd.GetGlobalPosition().z + randZ };

            // indicator
            indicator.SetActive(true);
            indicator.GetTransform().SetPosition(endPos);
        }
    }
    else
    {
        API_Vector3 pos;
        pos.x = Lerp(startPos.x, endPos.x, fallingTime / maxFallingTime);
        pos.y = Lerp(startPos.y, endPos.y, fallingTime / maxFallingTime);
        pos.z = Lerp(startPos.z, endPos.z, fallingTime / maxFallingTime);

        gameObject.GetTransform().SetPosition(pos);
        fallingTime += Time::GetDeltaTime();

        //Finished Movement
        if (fallingTime > maxFallingTime)
        {
            cd = rand() % (maxTime - minTime) + minTime;
            moving = false;
            gameObject.GetTransform().SetPosition({ 0, 7777, 0 });
            indicator.SetActive(false);
        }
    }
}

void MeteoriteTrap::OnCollisionEnter(API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();

    if (detectionTag == "Player")
    {
        PlayerStats* playerStats = (PlayerStats*)other.GetGameObject().GetScript("PlayerStats");
        playerStats->TakeDamage(50.0f, 1000.0f);
    }
}

float MeteoriteTrap::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}