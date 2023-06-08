#include "ExclamationMark.h"

HELLO_ENGINE_API_C ExclamationMark* CreateExclamationMark(ScriptToInspectorInterface* script)
{
	ExclamationMark* classInstance = new ExclamationMark();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);

	script->AddDragBoxGameObject("Parent", &classInstance->_pull);

	return classInstance;
}

void ExclamationMark::Start()
{
	_count = 2.0f;

	//_pull = gameObject.GetParent();
}

void ExclamationMark::Update()
{
	if (_count > 0)
	{
		_count -= Time::GetDeltaTime();
		if (_count <= 0)
		{
			gameObject.SetParent(_pull);
			gameObject.SetActive(false);
		}
	}
}

void ExclamationMark::OnEnable()
{
	_count = 2.0f;
}

void ExclamationMark::OnDisable()
{
	gameObject.SetParent(_pull);
}
