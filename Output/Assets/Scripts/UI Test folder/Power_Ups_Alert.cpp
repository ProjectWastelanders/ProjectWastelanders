#include "Power_Ups_Alert.h"
HELLO_ENGINE_API_C Power_Ups_Alert* CreatePower_Ups_Alert(ScriptToInspectorInterface* script)
{
	Power_Ups_Alert* classInstance = new Power_Ups_Alert();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIImage("Alert_Material", &classInstance->power_up_Alert);
	script->AddDragBoxTextureResource("SPEED_INCREASE", &classInstance->textures_Power_up[0]);
	script->AddDragBoxTextureResource("FIRERATE_INCREASE", &classInstance->textures_Power_up[1]);
	script->AddDragBoxTextureResource("SHIELD", &classInstance->textures_Power_up[2]);
	//script->AddDragBoxTextureResource("MAX AMMO", &classInstance->textures_Power_up[3]);
	//
	script->AddDragBoxTextureResource("SLOW_TIME", &classInstance->textures_Power_up[4]);
	return classInstance;
}

void Power_Ups_Alert::Start()
{
	power_up_Alert.SetOpacity(0);
}

void Power_Ups_Alert::Update()
{
	//if (Reset == true)
	//{
	//	timer_Active = 1.8;
	//	active = false;
	//	opacity_Alert = 1;
	//	Reset = false;
	//}

	//if (Reset == false)
	//{
		if (timer_Active < 1.8f)
		{
			timer_Active -= Time::GetDeltaTime();
			if (timer_Active < 1.8 && active == true)
			{
				power_up_Alert.SetOpacity(opacity_Alert = opacity_Alert + 0.05);
			}

			if (timer_Active < 1.3)
			{
				active = false;
				power_up_Alert.SetOpacity(opacity_Alert = opacity_Alert - 0.007);
			}

			if (opacity_Alert <= 0)
			{
				timer_Active = 1.8;
			}
		}
	//}
}

void Power_Ups_Alert::Swap_PowerUp_Texture(int Power_Up)
{
	timer_Active -= Time::GetDeltaTime();
	active = true;

	switch (Power_Up)
	{
	case 0:
		power_up_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures_Power_up[0]);
		break;
	case 1:
		power_up_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures_Power_up[1]);
		break;
	case 2:
		power_up_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures_Power_up[2]);
		break;
	case 3:
		break;
	case 4:
		power_up_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures_Power_up[4]);
		break;
	default:
		break;
	}
}
