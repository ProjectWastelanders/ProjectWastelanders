#include "Headers.h"
#include "CommandChangeTransform.h"

CommandChangeTransform::CommandChangeTransform(float4x4& newGlobalMatrix, float4x4& currentGlobalMatrix, bool ignoreRotation, std::function<void(float4x4&, bool, bool)> function)
{
	_newGlobalMatrix = newGlobalMatrix;
	_currentGlobalMatrix = currentGlobalMatrix;
	_ignoreRotation = ignoreRotation;
	_func = function;
}

CommandChangeTransform::~CommandChangeTransform()
{
}

void CommandChangeTransform::Undo()
{
	_func(_currentGlobalMatrix, _ignoreRotation, false);
}

void CommandChangeTransform::Redo()
{
	_func(_newGlobalMatrix, _ignoreRotation, false);
}
