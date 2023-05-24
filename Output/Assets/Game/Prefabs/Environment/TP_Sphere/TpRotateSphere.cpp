#include "TpRotateSphere.h"
HELLO_ENGINE_API_C TpRotateSphere* CreateTpRotateSphere(ScriptToInspectorInterface* script)
{
	TpRotateSphere* classInstance = new TpRotateSphere();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragFloat("Multiplier", &classInstance->timeMult);
	return classInstance;
}

void TpRotateSphere::Start()
{
	timeCount = 0.0f;
}
void TpRotateSphere::Update()
{
	timeCount += Time::GetDeltaTime();
	gameObject.GetTransform().Rotate( 0, timeCount * timeMult, 0 );
}