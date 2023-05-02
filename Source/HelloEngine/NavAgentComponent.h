#pragma once
#include "Component.h"

struct NavAgent;
class Pathfinder;

class ComponentAgent : public Component
{
public: 
	ComponentAgent(GameObject* gameObject);

	~ComponentAgent();

#ifdef STANDALONE
	void OnEditor() override;
#endif // STANDALONE

	void Serialization(json& j) override;
	void DeSerialization(json& j) override;

	void SetTarget(float3 targetPos);

	void MoveToTarget();

	void Stop();

	bool IsMoving() { return _move; }

public:
	NavAgent* agentProperties = nullptr;

private:
	Pathfinder* _pathfinder = nullptr;

	float3 _target = { 0,0,0 };
	int _indexInTheModule = 0;
	bool _move = false;
};

