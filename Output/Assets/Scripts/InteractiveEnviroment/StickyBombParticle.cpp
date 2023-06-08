#include "StickyBombParticle.h"
HELLO_ENGINE_API_C StickyBombParticle* CreateStickyBombParticle(ScriptToInspectorInterface* script)
{
    StickyBombParticle* classInstance = new StickyBombParticle();
    //Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    return classInstance;
}

void StickyBombParticle::Start()
{

}
void StickyBombParticle::Update()
{
    if (time > 0.0f)
    {
        time -= Time::GetDeltaTime();
        if (time <= 0.0f)
        {
            gameObject.SetActive(false);
        }
    }
}