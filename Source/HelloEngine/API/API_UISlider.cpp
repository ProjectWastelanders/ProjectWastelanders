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

    /*if (_UICheckBox->checkActive)
    {
        return _UICheckBox->State == CheckboxState::ONPRESSACTIVE;
    }
    else
    {
        return _UICheckBox->State == CheckboxState::ONPRESS;
    }*/
    //return _UIButton->State == CheckboxState::ONPRESS;
}

bool API::API_UISlider::OnHold()
{
    if (!_UISlider)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }

    /*if (_UICheckBox->checkActive)
    {
        return _UICheckBox->State == CheckboxState::ONPRESSACTIVE;
    }
    else
    {
        return _UICheckBox->State == CheckboxState::ONPRESS;
    }*/
    //return _UICheckBox->State == CheckboxState::ONHOLD;
}

bool API::API_UISlider::OnHovered()
{
    if (!_UISlider)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    /*if (_UICheckBox->checkActive)
    {
        return _UICheckBox->State == CheckboxState::HOVEREDACTIVE;
    }
    else
    {
        return _UICheckBox->State == CheckboxState::HOVERED;
    }*/
}

int API::API_UISlider::GetValue()
{
    return _UISlider->perCent;
}