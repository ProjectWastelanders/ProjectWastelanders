#include "Headers.h"
#include "ModuleLayers.h"
#include "API_UICheckBox.h"
#include "ComponentUICheckbox.h"


API::API_UICheckBox::API_UICheckBox()
{
}

API::API_UICheckBox::~API_UICheckBox()
{
}

API::API_GameObject API::API_UICheckBox::GetGameObject()
{
    if (!_UICheckBox)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UICheckBox");
        return *ModuleLayers::emptyAPIGameObject;
    }
    API_GameObject returnGO;
    returnGO.SetGameObject(_UICheckBox->GetGameObject());
    return returnGO;
}

ComponentUICheckbox* API::API_UICheckBox::GetComponent()
{
    return _UICheckBox;
}

void API::API_UICheckBox::SetComponent(ComponentUICheckbox* component)
{
    _UICheckBox = component;
}

bool API::API_UICheckBox::OnPress()
{
    if (!_UICheckBox)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }

    if (_UICheckBox->checkActive)
    {
        return _UICheckBox->State == CheckboxState::ONPRESSACTIVE;
    }
    else
    {
        return _UICheckBox->State == CheckboxState::ONPRESS;
    }
    //return _UIButton->State == CheckboxState::ONPRESS;
}

bool API::API_UICheckBox::OnHold()
{
    if (!_UICheckBox)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }

    if (_UICheckBox->checkActive)
     {
         return _UICheckBox->State == CheckboxState::ONPRESSACTIVE;
     }
     else
     {
         return _UICheckBox->State == CheckboxState::ONPRESS;
     }
    //return _UICheckBox->State == CheckboxState::ONHOLD;
}

bool API::API_UICheckBox::OnHovered()
{
    if (!_UICheckBox)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    if (_UICheckBox->checkActive)
    {
        return _UICheckBox->State == CheckboxState::HOVEREDACTIVE;
    }
    else
    {
        return _UICheckBox->State == CheckboxState::HOVERED;
    }
}

bool API::API_UICheckBox::getIsActive()
{
    if (!_UICheckBox)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    return _UICheckBox->checkActive;
}

void API::API_UICheckBox::SetActive(bool set)
{
    if (!_UICheckBox)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return;
    }
    _UICheckBox->checkActive = set;
}