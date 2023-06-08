#include "ExclamationPool.h"

int ExclamationPool::_marksCount = 10;
int ExclamationPool::_currentMark = 0;
API_GameObject ExclamationPool::_marks[10];

HELLO_ENGINE_API_C ExclamationPool* CreateExclamationPool(ScriptToInspectorInterface* script)
{
	ExclamationPool* classInstance = new ExclamationPool();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	
	script->AddDragBoxGameObject("mark1", &classInstance->mark1);
	script->AddDragBoxGameObject("mark2", &classInstance->mark2);
	script->AddDragBoxGameObject("mark3", &classInstance->mark3);
	script->AddDragBoxGameObject("mark4", &classInstance->mark4);
	script->AddDragBoxGameObject("mark5", &classInstance->mark5);
	script->AddDragBoxGameObject("mark6", &classInstance->mark6);
	script->AddDragBoxGameObject("mark7", &classInstance->mark7);
	script->AddDragBoxGameObject("mark8", &classInstance->mark8);
	script->AddDragBoxGameObject("mark9", &classInstance->mark9);
	script->AddDragBoxGameObject("mark10", &classInstance->mark10);

	return classInstance;
}

void ExclamationPool::Start()
{
	_marks[0] = mark1;
	_marks[1] = mark2;
	_marks[2] = mark3;
	_marks[3] = mark4;
	_marks[4] = mark5;
	_marks[5] = mark6;
	_marks[6] = mark7;
	_marks[7] = mark8;
	_marks[8] = mark9;
	_marks[9] = mark10;

	_marksCount = 10;
	_currentMark = 0;

	gameObject.GetTransform().SetPosition(API_Vector3(_marksCount, _marksCount, _marksCount));
}

void ExclamationPool::ActivateMark(API_GameObject parent)
{
	API_Vector3 parentPos = parent.GetTransform().GetGlobalPosition();

	_marks[_currentMark].GetTransform().SetPosition(0, 3, 0);

	_marks[_currentMark].SetParent(parent);

	_marks[_currentMark].SetActive(true);

	if (++_currentMark >= _marksCount)
		_currentMark = 0;
}