#include "Weapon_Select.h"
HELLO_ENGINE_API_C Weapon_Select* CreateWeapon_Select(ScriptToInspectorInterface* script)
{
	Weapon_Select* classInstance = new Weapon_Select();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIButton("Weapon 1", &classInstance->weapon1);
	script->AddDragBoxUIButton("Weapon 2", &classInstance->weapon2);
	script->AddDragBoxUIButton("Weapon 3", &classInstance->weapon3);
	script->AddDragBoxUIButton("Weapon 4", &classInstance->weapon4);

	script->AddDragBoxUIImage("Weapon Info", &classInstance->infoWeapon);

	script->AddDragBoxTextureResource("Info Weapon 1", &classInstance->info1);
	script->AddDragBoxTextureResource("Info Weapon 2", &classInstance->info2);
	script->AddDragBoxTextureResource("Info Weapon 3", &classInstance->info3);
	script->AddDragBoxTextureResource("Info Weapon 4", &classInstance->info4);
	return classInstance;
}

void Weapon_Select::Start()
{
	indexLevles = 3;

	if (API_QuickSave::GetBool("semiautomaticUnlocked"))
		weapon1.SetBlocked(true);
	else
		weapon1.SetBlocked(false);

	if (API_QuickSave::GetBool("automaticUnlocked"))
		weapon2.SetBlocked(true);
	else
		weapon2.SetBlocked(false);

	if (API_QuickSave::GetBool("burstUnlocked"))
		weapon3.SetBlocked(true);
	else
		weapon3.SetBlocked(false);

	if (API_QuickSave::GetBool("shotgunUnlocked"))
		weapon4.SetBlocked(true);
	else
		weapon4.SetBlocked(false);
}
void Weapon_Select::Update()
{
	if ((Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > 10000 && isPress) && indexLevles >= 1)
	{
		isPress = false;
		indexLevles--;
	}
	else if ((Input::GetGamePadButton(GamePadButton::BUTTON_UP) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < -10000 && isPress) && indexLevles <= 2)
	{
		isPress = false;
		indexLevles++;
	}
	else if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > -10000 && Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < 10000)
	{
		isPress = true;
	}

	switch (indexLevles)
	{
	case 3:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info1);
		break;
	case 2:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info2);
		break;
	case 1:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info3);
		break;
	case 0:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info4);
		break;
	default:
		break;
	}
}