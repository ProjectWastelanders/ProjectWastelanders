#include "ExclamationMarkPool.h"

HELLO_ENGINE_API_C ExclamationMarkPool* CreateExclamationMarkPull(ScriptToInspectorInterface* script)
{
	ExclamationMarkPool* classInstance = new ExclamationMarkPool();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ExclamationMarkPool::Start()
{
	//_marksCount = gameObject.GetChildren(_marks);

	gameObject.GetTransform().SetPosition(API_Vector3(_marksCount, _marksCount, _marksCount));
}

void ExclamationMarkPool::ActivateMark(API_GameObject parent)
{
	_marks[_currentMark].SetParent(parent);

	API_Vector3 parentPos = parent.GetTransform().GetGlobalPosition();

	_marks[_currentMark].GetTransform().SetPosition(API_Vector3(parentPos.x, parentPos.y + 2, parentPos.z));

	_marks[_currentMark].SetActive(true);

	if (++_currentMark >= _marksCount)
		_currentMark = 0;
}
