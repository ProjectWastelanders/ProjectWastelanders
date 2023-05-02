#include "Headers.h"
#include "ModuleLayers.h"
#include "API_UIButton.h"
#include "ComponentUIButton.h"

API::API_UIButton::API_UIButton()
{
}

API::API_UIButton::~API_UIButton()
{
}

API::API_GameObject API::API_UIButton::GetGameObject()
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UIButton");
        return *ModuleLayers::emptyAPIGameObject;
    }
    API_GameObject returnGO;
    returnGO.SetGameObject(_UIButton->GetGameObject());
    return returnGO;
}

bool API::API_UIButton::OnPress()
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }

     if (_UIButton->isBlocked)
     {
         return _UIButton->State == ButtonState::ONPRESSBLOCKED;
     }
     else
     {
         return _UIButton->State == ButtonState::ONPRESS;
     }
    //return _UIButton->State == ButtonState::ONPRESS;
}

bool API::API_UIButton::OnHold()
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }

    /* if (_UIButton->isBlocked)
     {
         return _UIButton->State == ButtonState::ONHOLDBLOCKED;
     }
     else
     {
         return _UIButton->State == ButtonState::ONHOLD;
     }*/
    return _UIButton->State == ButtonState::ONHOLD;
}

bool API::API_UIButton::OnHovered()
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    if (_UIButton->isBlocked)
    {
        return _UIButton->State == ButtonState::HOVEREDBLOCKED;
    }
    else
    {
        return _UIButton->State == ButtonState::HOVERED;
    }
}

ComponentUIButton* API::API_UIButton::GetComponent()
{
    return _UIButton;
}

void API::API_UIButton::SetComponent(ComponentUIButton* component)
{
    _UIButton = component;
}


bool API::API_UIButton::getIsBlocked()
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    return _UIButton->isBlocked;
}

void API::API_UIButton::SetBlocked(bool set)
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return;
    }
    _UIButton->isBlocked = set;
}

bool API::API_UIButton::OnRelease()
{
    if (!_UIButton)
    {
        Engine::Console::S_Log("Trying to acces a NULLPTR UI Button");
        return false;
    }
    return _UIButton->isReleased;
}

