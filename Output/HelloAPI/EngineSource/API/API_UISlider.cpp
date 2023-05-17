#include "Headers.h"
#include "ModuleLayers.h"
#include "API_UISlider.h"
#include "ComponentUISlider.h"


API::API_UISlider::API_UISlider()
{
}

API::API_UISlider::~API_UISlider()
{
}

API::API_GameObject API::API_UISlider::GetGameObject()
{
    if (!_UISlider)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UICheckBox");
        return *ModuleLayers::emptyAPIGameObject;
    }
    API_GameObject returnGO;
    returnGO.SetGameObject(_UISlider->GetGameObject());
    return returnGO;
}

ComponentUISlider* API::API_UISlider::GetComponent()
{
    return _UISlider;
}

void API::API_UISlider::SetComponent(ComponentUISlider* component)
{
    _UISlider = component;
}

bool API::API_UISlider::OnPress()
{
    if (!_UISlider)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    else
    {
        return _UISlider->State == SliderState::ONPRESS;
    }
}

bool API::API_UISlider::OnHold()
{
    if (!_UISlider)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    else
    {
        return _UISlider->State == SliderState::ONHOLD;
    }
}

bool API::API_UISlider::OnHovered()
{
    if (!_UISlider)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    else
    {
        return _UISlider->State == SliderState::HOVER;
    }
}

int API::API_UISlider::GetValue()
{
    _UISlider->CalculPerCent();
    return _UISlider->perCent;
}