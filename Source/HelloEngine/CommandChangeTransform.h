#pragma once
#include "Command.h"
class CommandChangeTransform : public Command
{
public:
	CommandChangeTransform(float4x4& newGlobalMatrix, float4x4& currentGlobalMatrix, bool ignoreRotation, std::function<void(float4x4&, bool, bool)>);

	~CommandChangeTransform();

	void Undo() override;

	void Redo() override;

private:
	float4x4 _currentGlobalMatrix;
	float4x4 _newGlobalMatrix;
	bool _ignoreRotation = false;
	std::function<void(float4x4&, bool, bool)> _func;
};

