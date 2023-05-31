#include "Map_Swap_LVL3.h"
HELLO_ENGINE_API_C Map_Swap_LVL3* CreateMap_Swap_LVL3(ScriptToInspectorInterface* script)
{
	Map_Swap_LVL3* classInstance = new Map_Swap_LVL3();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxMaterialComponent("Map Material", &classInstance->map_LVL3);
	script->AddDragBoxGameObject("Player GO", &classInstance->playerGO);
	script->AddDragBoxTextureResource("Map 1 Image", &classInstance->map1);
	script->AddDragBoxTextureResource("Map 2 Image", &classInstance->map2);

	return classInstance;
}

void Map_Swap_LVL3::Start()
{

}
void Map_Swap_LVL3::Update()
{
	if (playerGO.GetTransform().GetLocalPosition().y > 20 )
	{
		map_LVL3.ChangeAlbedoTexture(map2);
	}
	else
	{
		map_LVL3.ChangeAlbedoTexture(map1);
	}
}