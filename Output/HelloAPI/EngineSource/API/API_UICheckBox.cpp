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