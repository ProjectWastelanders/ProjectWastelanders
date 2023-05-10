#include "Coleccionable_buttons.h"
#include "InteractiveEnviroment/OpenMenuInterruptor.h"
#include "Player/PlayerMove.h"

HELLO_ENGINE_API_C Coleccionable_buttons* CreateColeccionable_buttons(ScriptToInspectorInterface* script)
{
	Coleccionable_buttons* classInstance = new Coleccionable_buttons();

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("interruptor_Colec_GO", &classInstance->interruptor_Colec_GO);

	script->AddDragBoxGameObject("Colleccionable Panel", &classInstance->coleccionable_panel);


	script->AddDragBoxUICheckBox("CheckBox 1", &classInstance->checkBox[0]);
	script->AddDragBoxUICheckBox("CheckBox 2", &classInstance->checkBox[1]);
	script->AddDragBoxUICheckBox("CheckBox 3", &classInstance->checkBox[2]);

	script->AddDragBoxUIImage("level", &classInstance->level_selected);

	script->AddDragBoxTextureResource("Song 1 texture", &classInstance->levels[0]);
	script->AddDragBoxTextureResource("Song 2 texture", &classInstance->levels[1]);
	script->AddDragBoxTextureResource("Song 3 texture", &classInstance->levels[2]);
	
	script->AddDragBoxGameObject("Casete 1 GO", &classInstance->casetes[0]);
	script->AddDragBoxGameObject("Casete 2 GO", &classInstance->casetes[1]);
	script->AddDragBoxGameObject("Casete 3 GO", &classInstance->casetes[2]);

	
	
	script->AddDragBoxGameObject("Player", &classInstance->playerStorageGO);
	return classInstance;
}

void Coleccionable_buttons::Start()
{
	for (int i = 0; i < 3; i++)
	{
		casetes[i].SetActive(false);
	}

	playerStorage = (PlayerStorage*)playerStorageGO.GetScript("PlayerStorage");
	if (playerStorage == nullptr) Console::Log("PlayerStorage missing in Coleccionable_buttons Script.");

	interruptor_Colec = (OpenMenuInterruptor*)interruptor_Colec_GO.GetScript("OpenMenuInterruptor");
	if (interruptor_Colec == nullptr) Console::Log("OpenMenuInterruptor missing in Coleccionable_buttons Script");

	playermovment = (PlayerMove*)playerStorageGO.GetScript("PlayerMove");
	if (playermovment == nullptr) Console::Log("PlayerMove missing in Coleccionable_buttons Script");
}

void Coleccionable_buttons::Update()
{

	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN /* && coleccionable_panel.IsActive()*/)
	{
		if (!interruptor_Colec) return;
		Input::HandleGamePadButton(GamePadButton::BUTTON_B);
		// IT'S CORRECT DON'T REMOVE NOTHING

		interruptor_Colec->menuPanel.SetActive(true); // can set false if is not true
		interruptor_Colec->menuPanel.SetActive(false);
		if (playermovment) playermovment->openingChest = false;
		interruptor_Colec->open = false;
		return;
	}//


	if (playerStorage)
	{
		for (int i = 0; i < 3; i++)
		{
			//buttons[i].SetBlocked(true);
			if (checkBox[i].OnHovered())
			{
				switch (i)
				{
				case 0:
					level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[0]);
					Level1selected(playerStorage->casette1Picked, playerStorage->casette2Picked, playerStorage->casette3Picked);
					break;
				case 1:
					level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[1]);
					Level1selected(playerStorage->casette1Picked, playerStorage->casette2Picked, playerStorage->casette3Picked);
					break;
				case 2:
					level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[2]);
					Level1selected(playerStorage->casette1Picked, playerStorage->casette2Picked, playerStorage->casette3Picked);
					break;
				default:
					break;
				}

			}
			if (playerStorage->casette1Picked == true && playerStorage->casette2Picked == true && playerStorage->casette3Picked == true)
			{
				if (checkBox[i].OnPress())
				{
					switch (i)
					{
					case 0:
						Audio::Event("L1_collectable3");
						Audio::StopEvent(song2);
						Audio::StopEvent(song3);
						break;
					case 1:
						Audio::Event("L2_collectable3");
						Audio::StopEvent(song1);
						Audio::StopEvent(song3);
						break;
					case 2:
						Audio::Event("L3_collectable3");
						Audio::StopEvent(song2);
						Audio::StopEvent(song1);
						break;
					default:
						break;
					}
				}

			}

			if (checkBox[i].OnPress())
			{
				switch (i)
				{
				case 0:
					checkBox[1].SetActive(false);
					checkBox[2].SetActive(false);
					break;
				case 1:
					checkBox[0].SetActive(false);
					checkBox[2].SetActive(false);
					break;
				case 2:
					checkBox[0].SetActive(false);
					checkBox[1].SetActive(false);
					break;
				default:
					break;
				}
			}
		}
	
	}

}

void Coleccionable_buttons::Level1selected( bool casete1, bool casete2, bool casete3)
{
	


	if (casete1 == true)
	{
		casetes[0].SetActive(true);
	}
	else
	{
		casetes[0].SetActive(false);
	}
	
	if (casete2 == true)
	{
		casetes[1].SetActive(true);
	}
	else
	{
		casetes[1].SetActive(false);
	}
	
	if (casete3 == true)
	{
		casetes[2].SetActive(true);
	}
	else
	{
		casetes[2].SetActive(false);
	}

}

void Coleccionable_buttons::Level2selected( bool casete1, bool casete2, bool casete3)
{

	if (casete1 == true)
	{
		casetes[0].SetActive(true);

	}
	else
	{
		casetes[0].SetActive(false);
	}

	if (casete2 == true)
	{
		casetes[1].SetActive(true);
	}
	else
	{
		casetes[1].SetActive(false);
	}

	if (casete3 == true)
	{
		casetes[2].SetActive(true);
	}
	else
	{
		casetes[2].SetActive(false);
	}


}

void Coleccionable_buttons::Level3selected( bool casete1, bool casete2, bool casete3)
{


	if (casete1 == true)
	{
		casetes[0].SetActive(true);
	}
	else
	{
		casetes[0].SetActive(false);
	}

	if (casete2 == true)
	{
		casetes[1].SetActive(true);
	}
	else
	{
		casetes[1].SetActive(false);
	}

	if (casete3 == true)
	{
		casetes[2].SetActive(true);
	}
	else
	{
		casetes[2].SetActive(false);
	}
}

