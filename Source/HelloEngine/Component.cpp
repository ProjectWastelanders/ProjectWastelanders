#include "Headers.h"
#include "GameObject.h"

Component::Component(GameObject* gameObject) : _gameObject(gameObject)
{
}

void Component::Enable()
{
	_isEnabled = true;
	if (_gameObject->IsActive())
		OnEnable();
}

void Component::Disable()
{
	_isEnabled = false;
	if (_gameObject->IsActive())
		OnDisable();
}