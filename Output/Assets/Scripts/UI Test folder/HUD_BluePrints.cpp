#include "HUD_BluePrints.h"

HELLO_ENGINE_API_C HUD_BluePrints* CreateHUD_BluePrints(ScriptToInspectorInterface* script)
{
	HUD_BluePrints* classInstance = new HUD_BluePrints();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIImage("Image_Upgrade", &classInstance->material_upgrade);
	script->AddDragBoxUIImage("Image_New_Weapon", &classInstance->material_new_weapon);
	script->AddDragBoxUIImage("Image_Special_Weapon", &classInstance->material_special_weapon);
	
	//background
	script->AddDragBoxMaterialComponent("Material_Background_Upgrade", &classInstance->material_Background_1);
	script->AddDragBoxMaterialComponent("Material_Background_New_Weapon", &classInstance->material_Background_2);
	script->AddDragBoxMaterialComponent("Material_Background_Special_Weapon", &classInstance->material_Background_3);

	//text
	script->AddDragBoxUIText("Text_Upgrade", &classInstance->text_upgrade);
	script->AddDragBoxUIText("Text_New_Weapon", &classInstance->text_new_weapon);
	script->AddDragBoxUIText("Text_Special_Weapon", &classInstance->text_special_weapon);

	//Textures Upgrade
	script->AddDragBoxTextureResource("Texture_Upgrade", &classInstance->upgrade_textures);
	//Textures NewWeapon
	script->AddDragBoxTextureResource("Texture_blueprint_Semiautomatic", &classInstance->new_weapon_textures[0]);
	script->AddDragBoxTextureResource("Texture_blueprint_Automatic", &classInstance->new_weapon_textures[1]);
	script->AddDragBoxTextureResource("Texture_blueprint_Burst", &classInstance->new_weapon_textures[2]);
	script->AddDragBoxTextureResource("Texture_blueprint_Shotgun", &classInstance->new_weapon_textures[3]);

	//Textures SpecialWeapon
	script->AddDragBoxTextureResource("Texture_Lanzallamas", &classInstance->special_weapon_textures[0]);
	script->AddDragBoxTextureResource("Texture_Ricochet", &classInstance->special_weapon_textures[1]);

	//

	return classInstance;
}

void HUD_BluePrints::Start()
{
	upgrade_movmentX = {-0.0032,0,0};
	new_weapon_movmentX = { -0.0032,0,0 };
	special_weapon_movmentX = { -0.0032,0,0 };

	upgrade_position_save = material_upgrade.GetGameObject().GetTransform().GetLocalPosition();
	new_weapon_position_save = material_new_weapon.GetGameObject().GetTransform().GetLocalPosition();
	special_weapon_position_save = material_special_weapon.GetGameObject().GetTransform().GetLocalPosition();
	text_upgrade_position_save = text_upgrade.GetGameObject().GetTransform().GetLocalPosition();
	text_new_weapon_position_save = text_new_weapon.GetGameObject().GetTransform().GetLocalPosition();
	text_special_weapon_position_save = text_special_weapon.GetGameObject().GetTransform().GetLocalPosition();

	Background_upgrade_position_save = material_Background_1.GetGameObject().GetTransform().GetLocalPosition();
	Background_new_weapon_position_save = material_Background_2.GetGameObject().GetTransform().GetLocalPosition();
 	Background_special_weapon_position_save = material_Background_3.GetGameObject().GetTransform().GetLocalPosition();

}
void HUD_BluePrints::Update()
{

	if (timer_upgrade < 1.8f)
	{
		timer_upgrade -= Time::GetDeltaTime();

		if (timer_upgrade >= 0.0f)
		{
			if (material_upgrade.GetGameObject().GetTransform().GetLocalPosition().x > 0.94)
			{
				material_upgrade.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
			if (text_upgrade.GetGameObject().GetTransform().GetLocalPosition().x > 0.84f)
			{
				text_upgrade.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
			if (material_Background_1.GetGameObject().GetTransform().GetLocalPosition().x > 0.9f)
			{
				material_Background_1.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
		}
		else if (material_upgrade.GetGameObject().GetTransform().GetLocalPosition().x <= 0.94)
		{
		material_upgrade.SetOpacity(opacity_upgrade = opacity_upgrade - 0.005);
		}
		if (opacity_upgrade <= 0)
		{
			material_upgrade.GetGameObject().GetTransform().SetPosition(upgrade_position_save);
			text_upgrade.GetGameObject().GetTransform().SetPosition(text_upgrade_position_save);
			material_Background_1.GetGameObject().GetTransform().SetPosition(Background_upgrade_position_save);
			opacity_upgrade = 1;
			timer_upgrade = 1.8f;
			material_upgrade.SetOpacity(opacity_upgrade);
		}
	}

	if (timer_new < 1.8f)
	{
		timer_new -= Time::GetDeltaTime();

		if (timer_new >= 0.0f)
		{

			if (material_new_weapon.GetGameObject().GetTransform().GetLocalPosition().x > 0.94)
			{
				material_new_weapon.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
			if (text_new_weapon.GetGameObject().GetTransform().GetLocalPosition().x > 0.84f)
			{
				text_new_weapon.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
			if (material_Background_2.GetGameObject().GetTransform().GetLocalPosition().x > 0.9f)
			{
				material_Background_2.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}

		}
		else if (material_new_weapon.GetGameObject().GetTransform().GetLocalPosition().x <= 0.94)
		{
			material_new_weapon.SetOpacity(opacity_new_weapon = opacity_new_weapon - 0.005);
		}
		if (opacity_new_weapon <= 0)
		{
			material_new_weapon.GetGameObject().GetTransform().SetPosition(new_weapon_position_save);
			text_new_weapon.GetGameObject().GetTransform().SetPosition(text_new_weapon_position_save);
			material_Background_2.GetGameObject().GetTransform().SetPosition(Background_new_weapon_position_save);
			opacity_new_weapon = 1;
			material_new_weapon.SetOpacity(opacity_new_weapon);
			timer_new = 1.8f;
		}
	}

	if (timer_special < 1.8f)
	{
		timer_special -= Time::GetDeltaTime();

			if (material_special_weapon.GetGameObject().GetTransform().GetLocalPosition().x > 0.94)
			{
				material_special_weapon.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
			if (text_special_weapon.GetGameObject().GetTransform().GetLocalPosition().x > 0.84f)
			{
				text_special_weapon.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
			if (material_Background_3.GetGameObject().GetTransform().GetLocalPosition().x > 0.9f)
			{
				material_Background_3.GetGameObject().GetTransform().Translate(upgrade_movmentX);
			}
		else if (material_special_weapon.GetGameObject().GetTransform().GetLocalPosition().x <= 0.94)
		{
			material_upgrade.SetOpacity(opacity_special_weapon = opacity_special_weapon - 0.005);
		
		}
		if (opacity_special_weapon <= 0)
		{
			material_special_weapon.GetGameObject().GetTransform().SetPosition(special_weapon_position_save);
			text_special_weapon.GetGameObject().GetTransform().SetPosition(text_special_weapon_position_save);
			material_Background_3.GetGameObject().GetTransform().SetPosition(Background_special_weapon_position_save);
			timer_special = 1.8f;
			opacity_special_weapon = 1;
			material_upgrade.SetOpacity(opacity_special_weapon);
		}
	}
}

void HUD_BluePrints::UpgradeAlert(int GetUpgrade)
{
	timer_upgrade -= Time::GetDeltaTime();

	switch (GetUpgrade)
	{
	case 0:
		material_upgrade.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(upgrade_textures);
		break;
	default:
		break;
	}
}

void HUD_BluePrints::New_WeaponAlert(int GetNewWeapon)
{
	timer_new -= Time::GetDeltaTime();
	switch (GetNewWeapon)
	{
	case 1:
		material_new_weapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(new_weapon_textures[0]);
		break;
	case 2:
		material_new_weapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(new_weapon_textures[1]);
		break;
	case 3:
		material_new_weapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(new_weapon_textures[2]);
		break;
	case 4:
		material_new_weapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(new_weapon_textures[3]);
		break;
	default:
		break;
	}
}

void HUD_BluePrints::Special_WeaponAlert(int GetSpecialWeapon)
{
	timer_special -= Time::GetDeltaTime();

	switch (GetSpecialWeapon)
	{
	case 5:
		material_special_weapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(special_weapon_textures[0]);
		break;
	case 6:
		material_special_weapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(special_weapon_textures[1]);
		break;
	default:
		break;
	}
}
