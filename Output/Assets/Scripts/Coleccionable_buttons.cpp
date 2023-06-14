#include "Coleccionable_buttons.h"
#include "InteractiveEnviroment/OpenMenuInterruptor.h"
#include "Player/PlayerMove.h"
#include "UI Test folder/HUB/HUB_UIManager.h"

HELLO_ENGINE_API_C Coleccionable_buttons* CreateColeccionable_buttons(ScriptToInspectorInterface* script)
{
	Coleccionable_buttons* classInstance = new Coleccionable_buttons();

	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxGameObject("interruptor_Colec_GO", &classInstance->interruptor_Colec_GO);

	script->AddDragBoxUIInput("Colleccionable Panel", &classInstance->coleccionable_panel);


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

	script->AddDragBoxGameObject("Tutorial Screen 1", &classInstance->tutorialScreens[0]);
	script->AddDragBoxGameObject("Tutorial Screen 2", &classInstance->tutorialScreens[1]);
	return classInstance;
}

void Coleccionable_buttons::Start()
{
	if (!API_QuickSave::GetBool("Colectables_Tutorial", false))
	{
		tutorial = new UITutorial(tutorialScreens, 2);
		tutorialScreens[0].SetActive(true);
		tutorialScreens[1].SetActive(false);
	}
	else 
	{
		tutorial = nullptr;
		tutorialScreens[0].SetActive(false);
		tutorialScreens[1].SetActive(false);
	}

	for (int i = 0; i < 3; i++)
	{
		casetes[i].SetActive(false);
	}
	casetes_levels.resize(12);

	casetes_levels[0] = API_QuickSave::GetBool("level1_casette1");
	casetes_levels[1] = API_QuickSave::GetBool("level1_casette2");
	casetes_levels[2] = API_QuickSave::GetBool("level1_casette3");

	casetes_levels[3] = API_QuickSave::GetBool("level2_casette1");
	casetes_levels[4] = API_QuickSave::GetBool("level2_casette2");
	casetes_levels[5] = API_QuickSave::GetBool("level2_casette3");

	casetes_levels[6] = API_QuickSave::GetBool("level3_casette1");
	casetes_levels[7] = API_QuickSave::GetBool("level3_casette2");
	casetes_levels[8] = API_QuickSave::GetBool("level3_casette3");

	casetes_levels[9] = casetes_levels[0] && casetes_levels[1] && casetes_levels[2];
	casetes_levels[10] = casetes_levels[3] && casetes_levels[4] && casetes_levels[5];
	casetes_levels[11] = casetes_levels[6] && casetes_levels[7] && casetes_levels[8];

	interruptor_Colec = (OpenMenuInterruptor*)interruptor_Colec_GO.GetScript("OpenMenuInterruptor");
	if (interruptor_Colec == nullptr) Console::Log("OpenMenuInterruptor missing in Coleccionable_buttons Script");

	playermovment = (PlayerMove*)playerStorageGO.GetScript("PlayerMove");
	if (playermovment == nullptr) Console::Log("PlayerMove missing in Coleccionable_buttons Script");
}

void Coleccionable_buttons::Update()
{
	if (tutorial != nullptr)
	{	
		coleccionable_panel.SetEnable(false);
		if (Input::GetGamePadButton(GamePadButton::BUTTON_A) == KeyState::KEY_DOWN)
		{
			if (!tutorial->NextScreen())
			{
				tutorial->Unable();
				delete tutorial;
				tutorial = nullptr;
				coleccionable_panel.SetEnable(true);
				API_QuickSave::SetBool("Colectables_Tutorial", true);
			}
			
		}

		return;
	}
	else
	{
		for (int i = 0; i < 2; ++i)
		{
			tutorialScreens[i].SetActive(false);
		}
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN /* && coleccionable_panel.IsActive()*/)
	{
		if (!interruptor_Colec) return;
		// IT'S CORRECT DON'T REMOVE NOTHING

		interruptor_Colec->menuPanel.SetActive(true); // can set false if is not true
		interruptor_Colec->menuPanel.SetActive(false);
		if (playermovment) playermovment->openingChest = false;
		interruptor_Colec->open = false;

		Audio::StopEvent(song1);
		Audio::StopEvent(song2);
		Audio::StopEvent(song3);
		HUB_UIManager::ClosePanel();

		return;
	}//


	for (int i = 0; i < 3; i++)
	{
		//buttons[i].SetBlocked(true);
		if (checkBox[i].OnHovered())
		{
			switch (i)
			{
			case 0:
				level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[0]);
				Level1selected(casetes_levels[0], casetes_levels[1], casetes_levels[2]);
				break;
			case 1:
				level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[1]);
				Level1selected(casetes_levels[3], casetes_levels[4], casetes_levels[5]);
				break;
			case 2:
				level_selected.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(levels[2]);
				Level1selected(casetes_levels[6], casetes_levels[7], casetes_levels[8]);
				break;
			default:
				break;
			}

		}

		if (checkBox[i].OnPress())
		{
			switch (i)
			{
			case 0:
				if (casetes_levels[9])
				{
					Audio::StopEvent(song1);
					Audio::StopEvent(song2);
					Audio::StopEvent(song3);
					if (!checkBox[i].getIsActive())
					{
						song1 = Audio::Event("L1_collectable3");
					}
				}

				break;
			case 1:
				if (casetes_levels[10])
				{
					Audio::StopEvent(song1);
					Audio::StopEvent(song2);
					Audio::StopEvent(song3);
					if (!checkBox[i].getIsActive())
					{
						song2 = Audio::Event("L2_collectable3");
					}
				}
				break;
			case 2:
				if (casetes_levels[11])
				{
					Audio::StopEvent(song1);
					Audio::StopEvent(song2);
					Audio::StopEvent(song3);
					if (!checkBox[i].getIsActive())
					{
						song3 = Audio::Event("L3_collectable3");
					}
				}
				break;
			default:
				break;
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

void Coleccionable_buttons::Level1selected(bool casete1, bool casete2, bool casete3)
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

void Coleccionable_buttons::Level2selected(bool casete1, bool casete2, bool casete3)
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

void Coleccionable_buttons::Level3selected(bool casete1, bool casete2, bool casete3)
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

