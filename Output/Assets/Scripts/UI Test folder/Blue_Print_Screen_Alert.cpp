#include "Blue_Print_Screen_Alert.h"
HELLO_ENGINE_API_C Blue_Print_Screen_Alert* CreateBlue_Print_Screen_Alert(ScriptToInspectorInterface* script)
{
	Blue_Print_Screen_Alert* classInstance = new Blue_Print_Screen_Alert();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIImage("Alert_Material", &classInstance->prints_Alert);
	script->AddDragBoxTextureResource("Upgrade_texture", &classInstance->textures[0]);
	script->AddDragBoxTextureResource("Semi_Alert_texture", &classInstance->textures[1]);
	script->AddDragBoxTextureResource("Automatic_Alert_texture", &classInstance->textures[2]);
	script->AddDragBoxTextureResource("Burst_Alert_texture", &classInstance->textures[3]);
	script->AddDragBoxTextureResource("Shotgun_Alert_texture", &classInstance->textures[4]);
	script->AddDragBoxTextureResource("Lanzallamas_Alert_texture", &classInstance->textures[5]);
	script->AddDragBoxTextureResource("Ricochet_Alert_texture", &classInstance->textures[6]);

	return classInstance;
}

void Blue_Print_Screen_Alert::Start()
{
	prints_Alert.SetOpacity(0);
}
void Blue_Print_Screen_Alert::Update()
{
	if (timer_Active < 1.8f)
	{
		timer_Active -= Time::GetDeltaTime();
		if (timer_Active < 1.8 && active == true)
		{
			prints_Alert.SetOpacity(opacity_Alert = opacity_Alert + 0.05);
		}
	
		if (timer_Active < 0.7)
		{
			active = false;
			prints_Alert.SetOpacity(opacity_Alert = opacity_Alert - 0.007);
		}

		if (opacity_Alert <= 0)
		{
			timer_Active = 1.8;
		}
	
	}
}

void Blue_Print_Screen_Alert::Swap_BluePrint_Texture(int Print)
{
	timer_Active -= Time::GetDeltaTime();
	active = true;
	//opacityFull = true;
	switch (Print)
	{
	case 0:
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[0]);
		break;																	
	case 1:																		
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[1]);
		break;																	
	case 2:																		
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[2]);
		break;																	
	case 3:																		
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[3]);
		break;																	
	case 4:																		
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[4]);
		break;																	
	case 5:																		
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[5]);
		break;																	
	case 6:																		
		prints_Alert.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(textures[6]);
		break;
	default:
		break;
	}
}

