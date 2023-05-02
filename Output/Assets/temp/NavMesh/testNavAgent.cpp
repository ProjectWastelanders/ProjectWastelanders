#include "testNavAgent.h"

HELLO_ENGINE_API_C testNavAgent* CreatetestNavAgent(ScriptToInspectorInterface* script)
{
	testNavAgent* classInstance = new testNavAgent();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void testNavAgent::Start()
{
	agent = gameObject.GetAgent();

	agent.SetDestination(API_Vector3(0, 0, 0));
}
void testNavAgent::Update()
{

}