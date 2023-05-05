#include "Headers.h"
#include "API_Agent.h"
#include "ModuleLayers.h"
#include "NavAgentComponent.h"
#include "ModuleNavMesh.h"

API::API_Agent::API_Agent()
{
}

API::API_Agent::~API_Agent()
{
}

API::API_GameObject API::API_Agent::GetGameObject()
{
	if (!_componentAgent)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR UIButton");
		return *ModuleLayers::emptyAPIGameObject;
	}

	API_GameObject returnGO;
	returnGO.SetGameObject(_componentAgent->GetGameObject());

	return returnGO;
}

void API::API_Agent::SetDestination(API_Vector3 des)
{
	if (!_componentAgent)
		return;

	_componentAgent->SetTarget(des);
	_componentAgent->MoveToTarget();
}

void API::API_Agent::Move()
{
	if (!_componentAgent)
		return;

	_componentAgent->MoveToTarget();
}

void API::API_Agent::Stop()
{
	if (!_componentAgent)
		return;

	_componentAgent->Stop();
}

void API::API_Agent::SetSpeed(float speed)
{
	if (!_componentAgent)
		return;

	_componentAgent->agentProperties->speed = speed;
}

float API::API_Agent::GetSpeed()
{
	if (!_componentAgent)
		return 0.0f;

	return _componentAgent->agentProperties->speed;
}

ComponentAgent* API::API_Agent::GetComponent()
{
	return _componentAgent;
}

void API::API_Agent::SetComponent(ComponentAgent* component)
{
	_componentAgent = component;
}