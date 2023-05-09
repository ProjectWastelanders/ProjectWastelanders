#include "Weapon_Select.h"
#include "../../Assets/Scripts/InteractiveEnviroment/OpenMenuInterruptor.h"
#include "../../Assets/Scripts/Player/PlayerMove.h"
HELLO_ENGINE_API_C Weapon_Select* CreateWeapon_Select(ScriptToInspectorInterface* script)
{
	Weapon_Select* classInstance = new Weapon_Select();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	script->AddDragBoxUIInput("Weapon Selected Panel", &classInstance->weaponSelectPanel);
	script->AddDragBoxUIInput("Proceed Panel", &classInstance->proceedPanel);

	script->AddDragBoxUIButton("Weapon 1", &classInstance->weapon1);
	script->AddDragBoxUIButton("Weapon 2", &classInstance->weapon2);
	script->AddDragBoxUIButton("Weapon 3", &classInstance->weapon3);
	script->AddDragBoxUIButton("Weapon 4", &classInstance->weapon4);

	script->AddDragBoxUIImage("Weapon Info", &classInstance->infoWeapon);

	script->AddDragBoxTextureResource("Info Weapon 1", &classInstance->info1);
	script->AddDragBoxTextureResource("Info Weapon 2", &classInstance->info2);
	script->AddDragBoxTextureResource("Info Weapon 3", &classInstance->info3);
	script->AddDragBoxTextureResource("Info Weapon 4", &classInstance->info4);

	script->AddDragBoxUIImage("Weapon Image", &classInstance->weaponImage);

	script->AddDragBoxTextureResource("Weapon Image 1", &classInstance->weaponImage1);
	script->AddDragBoxTextureResource("Weapon Image 2", &classInstance->weaponImage2);
	script->AddDragBoxTextureResource("Weapon Image 3", &classInstance->weaponImage3);
	script->AddDragBoxTextureResource("Weapon Image 4", &classInstance->weaponImage4);
	
	script->AddDragBoxUIButton("Proceed Button", &classInstance->proceedButton);

	script->AddDragBoxGameObject("Open Menu Interruptor", &classInstance->interruptorGO);
	script->AddDragBoxGameObject("Player", &classInstance->playerGO);
	return classInstance;
}

void Weapon_Select::Start()
{
	indexLevles = 3;
	inOpen = true;
	//onProceed = false;

	interruptor = (OpenMenuInterruptor*)interruptorGO.GetScript("OpenMenuInterruptor");
	if (interruptor == nullptr) Console::Log("OpenMenuInterruptor missing Level Select");
	playerMove = (PlayerMove*)playerGO.GetScript("PlayerMove");
	if (playerMove == nullptr) Console::Log("PlayerMove missing in ArmoryWeaponSelect Script with gunIndex 0.");
}
void Weapon_Select::Update()
{
	if (inOpen)
	{
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

		inOpen = false;
	}

	if (onProceed)
	{
		Console::Log("true");
	}
	else
	{
		Console::Log("false");
	}

	if (Input::GetGamePadButton(GamePadButton::BUTTON_B) == KeyState::KEY_DOWN)
	{
		if (onProceed)
		{
			onProceed = false;
			Console::Log("JUAN TE MATO");
			weaponSelectPanel.GetGameObject().SetActive(true);
			proceedPanel.GetGameObject().SetActive(false);
		}
		else
		{
			if (!interruptor) return;
			Input::HandleGamePadButton(GamePadButton::BUTTON_B);
			// IT'S CORRECT DON'T REMOVE NOTHING
			interruptor->menuPanel.SetActive(true); // can set false if is not true
			interruptor->menuPanel.SetActive(false);
			if (playerMove) playerMove->openingChest = false;
			interruptor->open = false;
		}
	}

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
		weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage1);
		break;
	case 2:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info2);
		weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage2);
		break;
	case 1:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info3);
		weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage3);
		break;
	case 0:
		infoWeapon.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(info4);
		weaponImage.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(weaponImage4);
		break;
	}

	if (weapon1.OnPress() || weapon2.OnPress() || weapon3.OnPress() || weapon4.OnPress())
	{
		weaponSelectPanel.GetGameObject().SetActive(false);
		proceedPanel.GetGameObject().SetActive(true);
		onProceed = true;
	}

	if (proceedButton.OnPress())
	{
		if (API_QuickSave::GetBool("level1Selected"))
		{
			Console::Log("Go To LVELV");
			scene = std::string("Level1.HScene");
			Scene::LoadScene(scene.c_str());
		}
		else if (API_QuickSave::GetBool("level2Selected"))
		{
			scene = std::string("Level2.HScene");
			Scene::LoadScene(scene.c_str());
		}
		else if (API_QuickSave::GetBool("level3Selected"))
		{
			scene = std::string("Level3.HScene");
			Scene::LoadScene(scene.c_str());
		}
		else if (API_QuickSave::GetBool("level4Selected"))
		{
			scene = std::string("Level4.HScene");
			Scene::LoadScene(scene.c_str());
		}
	}
}