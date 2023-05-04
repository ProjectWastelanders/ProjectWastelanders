#include "Feedback_Level_Select.h"

HELLO_ENGINE_API_C Feedback_Level_Select* CreateFeedback_Level_Select(ScriptToInspectorInterface* script)
{
	Feedback_Level_Select* classInstance = new Feedback_Level_Select();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
    script->AddDragFloat("Hold Button Time", &classInstance->maxOpenSelectTime);
    script->AddDragBoxUIImage("Hold Bar", &classInstance->chargingBar);
    script->AddDragBoxUIImage("Selected Feedback", &classInstance->isSelected);

    script->AddDragBoxUIInput("Hold Conjunt", &classInstance->chargingConjunt);

    script->AddDragBoxUICheckBox("Level 1", &classInstance->level1);
    script->AddDragBoxUICheckBox("Level 2", &classInstance->level2);
    script->AddDragBoxUICheckBox("Level 3", &classInstance->level3);
    script->AddDragBoxUICheckBox("Level 4", &classInstance->level4);

    script->AddDragBoxUIText("Text Casette", &classInstance->casettesNumberText);

    script->AddDragBoxGameObject("Check Main Mision", &classInstance->checkMainMision1);
    script->AddDragBoxGameObject("Check Secundary Mision 1", &classInstance->checkSecundaryMision1);
    script->AddDragBoxGameObject("Check Secundary Mision 2", &classInstance->checkSecundaryMision2);

    script->AddDragBoxTextureResource("Material Main mision 1", &classInstance->mainMision1);
    script->AddDragBoxTextureResource("Material Main mision 2", &classInstance->mainMision2);
    script->AddDragBoxTextureResource("Material Main mision 3", &classInstance->mainMision3);
    script->AddDragBoxTextureResource("Material Main mision 4", &classInstance->mainMision4);

    script->AddDragBoxTextureResource("Material Secundary mision 1_1", &classInstance->secundaryMision1_1);
    script->AddDragBoxTextureResource("Material Secundary mision 2_1", &classInstance->secundaryMision2_1);
    script->AddDragBoxTextureResource("Material Secundary mision 1_1", &classInstance->secundaryMision1_2);
    script->AddDragBoxTextureResource("Material Secundary mision 2_1", &classInstance->secundaryMision2_2);
    script->AddDragBoxTextureResource("Material Secundary mision 1_1", &classInstance->secundaryMision1_3);
    script->AddDragBoxTextureResource("Material Secundary mision 2_1", &classInstance->secundaryMision2_3);
    script->AddDragBoxTextureResource("Material Secundary mision 1_1", &classInstance->secundaryMision1_4);
    script->AddDragBoxTextureResource("Material Secundary mision 2_1", &classInstance->secundaryMision2_4);

    script->AddDragBoxGameObject("Map Image", &classInstance->mapImage);

    script->AddDragBoxTextureResource("Material Map 1", &classInstance->map1);
    script->AddDragBoxTextureResource("Material Map 2", &classInstance->map2);
    script->AddDragBoxTextureResource("Material Map 3", &classInstance->map3);
    script->AddDragBoxTextureResource("Material Map 4", &classInstance->map4);

    script->AddDragBoxGameObject("Planet Image", &classInstance->planetImage);

    script->AddDragBoxTextureResource("Material Planet 1", &classInstance->planet1);
    script->AddDragBoxTextureResource("Material Planet 2", &classInstance->planet2);
    script->AddDragBoxTextureResource("Material Planet 3", &classInstance->planet3);
    script->AddDragBoxTextureResource("Material Planet 4", &classInstance->planet4);

    script->AddDragBoxGameObject("Feedback Planet Name", &classInstance->planet_Name);

    script->AddDragBoxTextureResource("Material Planet Name 1", &classInstance->planetName1);
    script->AddDragBoxTextureResource("Material Planet Name 2", &classInstance->planetName2);
    script->AddDragBoxTextureResource("Material Planet Name 3", &classInstance->planetName3);
    script->AddDragBoxTextureResource("Material Planet Name 4", &classInstance->planetName4);
	return classInstance;
}

void Feedback_Level_Select::Start()
{
    indexLevles = 3;
    selecting = false;
    currentLevel = level1;
    currentLevel.SetActive(true);
    planet_Name.SetActive(false);
    LevelSelect();
    SetCurrentLevel();
}
void Feedback_Level_Select::Update()
{
    if ((Input::GetGamePadButton(GamePadButton::BUTTON_DOWN) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > 10000 && isPress) && indexLevles >= 1)
    {
        isPress = false;

        openSelecTime = 0;
        indexLevles--;
        SetCurrentLevel();
        chargingConjunt.GetGameObject().GetTransform().SetPosition(chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().x, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().y - 0.385, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().z);

        LevelSelect();

    }
    else if ((Input::GetGamePadButton(GamePadButton::BUTTON_UP) == KeyState::KEY_DOWN || Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < -10000 && isPress) && indexLevles <= 2)
    {
        isPress = false;

        openSelecTime = 0;
        indexLevles++;
        SetCurrentLevel();
        chargingConjunt.GetGameObject().GetTransform().SetPosition(chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().x, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().y + 0.385, chargingConjunt.GetGameObject().GetTransform().GetGlobalPosition().z);
        
        LevelSelect();
    }
    else if (Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) > -10000 && Input::GetGamePadAxis(GamePadAxis::AXIS_LEFTY) < 10000)
    {
        isPress = true;
    }

    OnCharge();
}

void Feedback_Level_Select::OnCollisionEnter(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        chargingBar.GetGameObject().SetActive(true);
    }
}

void Feedback_Level_Select::OnCollisionExit(API::API_RigidBody other)
{
    std::string detectionTag = other.GetGameObject().GetTag();
    if (detectionTag == "Player")
    {
        chargingBar.GetGameObject().SetActive(false);
    }
}

void Feedback_Level_Select::OnCharge()
{
    chargingBar.FillImage(openSelecTime / maxOpenSelectTime);

    if (selecting)
    {
        openSelecTime += Time::GetRealTimeDeltaTime();

        if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_UP || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_UP)
        {
            openSelecTime = 0;
            selecting = false;
        }
    }
    if (Input::GetGamePadButton(GamePadButton::BUTTON_X) == KeyState::KEY_DOWN || Input::GetKey(KeyCode::KEY_E) == KeyState::KEY_DOWN)
    {
        selecting = true;
    }

    if (openSelecTime / maxOpenSelectTime >= 1)
    {
        currentLevel.SetActive(true);
        chargingConjunt.GetGameObject().SetActive(false);
        isSelected.GetGameObject().SetActive(true);
        planet_Name.SetActive(true);

        switch (indexLevles)
        {
        case 3:
            isSelected.GetGameObject().GetTransform().SetPosition(isSelected.GetGameObject().GetTransform().GetGlobalPosition().x, 0.530, isSelected.GetGameObject().GetTransform().GetGlobalPosition().z);

            level2.SetActive(false);
            level3.SetActive(false);
            level4.SetActive(false);
            planet_Name.GetMaterialCompoennt().ChangeAlbedoTexture(planetName1);
            break;
        case 2:
            isSelected.GetGameObject().GetTransform().SetPosition(isSelected.GetGameObject().GetTransform().GetGlobalPosition().x, 0.145, isSelected.GetGameObject().GetTransform().GetGlobalPosition().z);
            level1.SetActive(false);
            level3.SetActive(false);
            level4.SetActive(false);
            planet_Name.GetMaterialCompoennt().ChangeAlbedoTexture(planetName2);
            break;
        case 1:
            isSelected.GetGameObject().GetTransform().SetPosition(isSelected.GetGameObject().GetTransform().GetGlobalPosition().x, -0.240, isSelected.GetGameObject().GetTransform().GetGlobalPosition().z);
            level1.SetActive(false);
            level2.SetActive(false);
            level4.SetActive(false);
            planet_Name.GetMaterialCompoennt().ChangeAlbedoTexture(planetName3);
            break;
        case 0:
            isSelected.GetGameObject().GetTransform().SetPosition(isSelected.GetGameObject().GetTransform().GetGlobalPosition().x, -0.625, isSelected.GetGameObject().GetTransform().GetGlobalPosition().z);
            level1.SetActive(false);
            level2.SetActive(false);
            level3.SetActive(false);
            planet_Name.GetMaterialCompoennt().ChangeAlbedoTexture(planetName4);
            break;
        default:
            break;
        }
    }
}

void Feedback_Level_Select::SetCurrentLevel()
{
    casettesNumber = 0;

    switch (indexLevles)
    {
    case 3:
        currentLevel = level1;
        mainMision.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(mainMision1);
        secMision1.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision1_1);
        secMision2.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision2_1);

        mapImage.GetMaterialCompoennt().ChangeAlbedoTexture(map1);
        planetImage.GetMaterialCompoennt().ChangeAlbedoTexture(planet1);

        if (API_QuickSave::GetBool("level1_casette1"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level1_casette2"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level1_casette3"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("mision_LikeThe80", false))
        {
            checkSecundaryMision1.SetActive(true);
        }
        else
        {
            checkSecundaryMision1.SetActive(false);
        }

        if (API_QuickSave::GetBool("mision_RageMonster", false))
        {
            checkSecundaryMision2.SetActive(true);
        }
        else
        {
            checkSecundaryMision2.SetActive(false);
        }

        break;
    case 2:
        currentLevel = level2;
        mainMision.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(mainMision2);
        secMision1.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision1_2);
        secMision2.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision2_2);

        mapImage.GetMaterialCompoennt().ChangeAlbedoTexture(map2);
        planetImage.GetMaterialCompoennt().ChangeAlbedoTexture(planet2);

        if (API_QuickSave::GetBool("level2_casette1"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level2_casette2"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level2_casette3"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("mision_Masacre", false))
        {
            checkSecundaryMision1.SetActive(true);
        }
        else
        {
            checkSecundaryMision1.SetActive(false);
        }

        if (API_QuickSave::GetBool("mision_Personal", false))
        {
            checkSecundaryMision2.SetActive(true);
        }
        else
        {
            checkSecundaryMision2.SetActive(false);
        }

        break;
    case 1:
        currentLevel = level3;
        mainMision.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(mainMision3);
        secMision1.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision1_3);
        secMision2.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision2_3);

        mapImage.GetMaterialCompoennt().ChangeAlbedoTexture(map3);
        planetImage.GetMaterialCompoennt().ChangeAlbedoTexture(planet3);

        if (API_QuickSave::GetBool("level3_casette1"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level3_casette2"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level3_casette3"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("mision_TheRule", false))
        {
            checkSecundaryMision1.SetActive(true);
        }
        else
        {
            checkSecundaryMision1.SetActive(false);
        }

        checkSecundaryMision2.SetActive(false);

        break;
    case 0:
        currentLevel = level4;
        mainMision.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(mainMision4);
        secMision1.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision1_4);
        secMision2.GetGameObject().GetMaterialCompoennt().ChangeAlbedoTexture(secundaryMision2_4);

        mapImage.GetMaterialCompoennt().ChangeAlbedoTexture(map4);
        planetImage.GetMaterialCompoennt().ChangeAlbedoTexture(planet4);

        if (API_QuickSave::GetBool("level4_casette1"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level4_casette2"))
        {
            casettesNumber++;
        }

        if (API_QuickSave::GetBool("level4_casette3"))
        {
            casettesNumber++;
        }

        checkSecundaryMision1.SetActive(false);
        checkSecundaryMision2.SetActive(false);

        break;
    default:
        break;
    }

    casettesNumberText.SetText(std::to_string(casettesNumber).c_str());
}

void Feedback_Level_Select::LevelSelect()
{
    if (currentLevel.getIsActive())
    {
        chargingConjunt.GetGameObject().SetActive(false);
        //isSelected.GetGameObject().SetActive(true);
    }
    else
    {
        chargingConjunt.GetGameObject().SetActive(true);
        //isSelected.GetGameObject().SetActive(false);
    }
}
