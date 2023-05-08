#include "Coleccionable_buttons.h"
HELLO_ENGINE_API_C Coleccionable_buttons* CreateColeccionable_buttons(ScriptToInspectorInterface* script)
{
	Coleccionable_buttons* classInstance = new Coleccionable_buttons();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUICheckBox("CheckBox 1", &classInstance->CheckBox[0]);
	script->AddDragBoxUICheckBox("CheckBox 2", &classInstance->CheckBox[1]);
	script->AddDragBoxUICheckBox("CheckBox 3", &classInstance->CheckBox[2]);

	script->AddDragBoxUIImage("level", &classInstance->level_selected);

	script->AddDragBoxTextureResource("Song 1 texture", &classInstance->levels[0]);
	script->AddDragBoxTextureResource("Song 2 texture", &classInstance->levels[1]);
	script->AddDragBoxTextureResource("Song 3 texture", &classInstance->levels[2]);
	
	script->AddDragBoxGameObject("Casete 1 GO", &classInstance->Casetes[0]);
	script->AddDragBoxGameObject("Casete 2 GO", &classInstance->Casetes[1]);
	script->AddDragBoxGameObject("Casete 3 GO", &classInstance->Casetes[2]);

	
	
	script->AddDragBoxGameObject("Player", &classInstance->playerStorageGO);
	return classInstance;
}

void Coleccionable_buttons::Start()
{
	level_selected.GetGameObject().SetActive(false);

	for (int i = 0; i < 3; i++)
	{
		Casetes[i].SetActive(false);
	}

	playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
	if (playerStorage == nullptr) Console::Log("PlayerStorage missing in Coleccionable_buttons Script.");
}

void Coleccionable_buttons::Update()
{
	for (int i = 0; i < 3; i++)
	{
		//buttons[i].SetBlocked(true);
		if (CheckBox[i].OnPress())
		{
			level_selected.GetGameObject().SetActive(true);
			switch (i)
			{
			case 0:
				level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[0]);
				Level1selected(levels[0], true, true, true/*playerStorage->casette1Picked, playerStorage->casette2Picked, playerStorage->casette3Picked*/);
				break;
			case 1:
				level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[1]);
				Level1selected(levels[1], true, true, true/*playerStorage->casette1Picked, playerStorage->casette2Picked, playerStorage->casette3Picked*/);
				break;
			case 2:
				level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[2]);
				Level1selected(levels[2], true, true, true/*playerStorage->casette1Picked, playerStorage->casette2Picked, playerStorage->casette3Picked */);
				break;
			default:
				break;
			}

		}

	}
}

void Coleccionable_buttons::Level1selected(int level1, bool casete1, bool casete2, bool casete3)
{
	
	CheckBox[1].SetActive(false);
	CheckBox[2].SetActive(false);

	if (casete1 == true)
	{
		Casetes[0].SetActive(true);
		Audio::Event("L1_collectable1");
	}
	else
	{
		Casetes[0].SetActive(false);
	}
	
	if (casete2 == true)
	{
		Casetes[1].SetActive(true);
		Audio::Event("L1_collectable2");
	}
	else
	{
		Casetes[1].SetActive(false);
	}
	
	if (casete3 == true)
	{
		Casetes[2].SetActive(true);
		Audio::Event("L1_collectable3");
	}
	else
	{
		Casetes[2].SetActive(false);
	}
}

void Coleccionable_buttons::Level2selected(int level2, bool casete1, bool casete2, bool casete3)
{
	CheckBox[0].SetActive(false);
	CheckBox[2].SetActive(false);

	if (casete1 == true)
	{
		Casetes[0].SetActive(true);
		Audio::Event("L2_collectable1");
	}
	else
	{
		Casetes[0].SetActive(false);
	}

	if (casete2 == true)
	{
		Casetes[1].SetActive(true);
		Audio::Event("L2_collectable2");
	}
	else
	{
		Casetes[1].SetActive(false);
	}

	if (casete3 == true)
	{
		Casetes[2].SetActive(true);
		Audio::Event("L2_collectable3");
	}
	else
	{
		Casetes[2].SetActive(false);
	}
}

void Coleccionable_buttons::Level3selected(int level3, bool casete1, bool casete2, bool casete3)
{
	CheckBox[0].SetActive(false);
	CheckBox[2].SetActive(false);

	if (casete1 == true)
	{
		Casetes[0].SetActive(true);
		Audio::Event("L3_collectable1");
	}
	else
	{
		Casetes[0].SetActive(false);
	}

	if (casete2 == true)
	{
		Casetes[1].SetActive(true);
		Audio::Event("L3_collectable2");
	}
	else
	{
		Casetes[1].SetActive(false);
	}

	if (casete3 == true)
	{
		Casetes[2].SetActive(true);
		Audio::Event("L3_collectable3");
	}
	else
	{
		Casetes[2].SetActive(false);
	}
}
