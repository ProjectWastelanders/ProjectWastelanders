#include "Headers.h"
#include "ComponentUIInput.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"
#include "ModuleLayers.h"
ComponentUIInput::ComponentUIInput(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_INPUT;

	_meshRenderer->Disable();

	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
}

ComponentUIInput::~ComponentUIInput()
{
}

void ComponentUIInput::InputUpdate()
{
	if (!_isEnabled)
	{
		for (int i = 0; i < _listButtons.size(); ++i)
		{
			if (_listButtons[i]->_type == Component::Type::UI_BUTTON)
			{
				ComponentUIButton* button = (ComponentUIButton*)_listButtons[i];
				button->State = ButtonState::NORMAL;
			}
		}
	}

	if (_listButtons.size() != 0 && _gameObject->IsActive() && _isEnabled)
	{
		if (ButtonSelected < _listButtons.size() - 1 && ((ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) > 10000 && isPress) || ModuleInput::S_GetGamePadButton(GamePad::BUTTON_DOWN) == KEY_DOWN))
		{
			ButtonSelected++;
			isPress = false;
		}
		else if (ButtonSelected > 0 && ((ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) < -10000 && isPress) || ModuleInput::S_GetGamePadButton(GamePad::BUTTON_UP) == KEY_DOWN))
		{
			ButtonSelected--;
			isPress = false;
		}
		else if (ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) > -10000 && ModuleInput::S_GetGamePadAxis(SDL_CONTROLLER_AXIS_LEFTY) < 10000)
		{
			isPress = true;
		}

		if (_listButtons[ButtonSelected]->IsEnabled() && _listButtons[ButtonSelected]->GetGameObject()->IsActive())
			_listButtons[ButtonSelected]->UpdateGamePadInput(true);

		for (int i = 0; i < _listButtons.size(); ++i)
		{
			if (i == ButtonSelected || !_listButtons[i]->IsEnabled() || !_listButtons[i]->GetGameObject()->IsActive())
				continue;
			_listButtons[i]->UpdateGamePadInput(false);
		}
	}
}

void ComponentUIInput::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	_j["listButtons"][0] = 0; // <- to prevent crashes on empty panels in DeSerialization method!
	for (size_t i = 0; i < _listButtons.size(); i++)
	{
		_j["listButtons"][i] = _listButtons[i]->GetGameObject()->GetID();
	}
	SaveMeshState(_j);
	j["Components"].push_back(_j);
}

void ComponentUIInput::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	std::vector<uint> listAux = j["listButtons"];

	for (size_t i = 0; i < listAux.size(); i++)
	{
		if (ModuleLayers::gameObjects.count(listAux[i]) == 0)
			continue;

		GameObject* GOAux = ModuleLayers::gameObjects[listAux[i]];
		if (GOAux == nullptr)
			continue;

		ComponentUIButton* CUIB = GOAux->GetComponent<ComponentUIButton>();

		if (CUIB != nullptr && CUIB->GetGameObject()->GetComponent<ComponentUIButton>())
		{
			_listButtons.push_back(CUIB);
		}

		ComponentUICheckbox* CUIC = GOAux->GetComponent<ComponentUICheckbox>();

		if (CUIC != nullptr && CUIC->GetGameObject()->GetComponent<ComponentUICheckbox>())
		{
			_listButtons.push_back(CUIC);
		}

		ComponentUISlider* CUIS = GOAux->GetComponent<ComponentUISlider>();

		if (CUIS != nullptr && CUIS->GetGameObject()->GetComponent<ComponentUISlider>())
		{
			_listButtons.push_back(CUIS);
		}
	}

	_gameObject->transform->ForceUpdate();
	
	LoadMeshState(j);

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}

#ifdef STANDALONE
void ComponentUIInput::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Panel", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	if (ImGui::Checkbox("Active##Panel", &_isEnabled))
		_isEnabled ? Enable() : Disable();

	ImGui::Text("");
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("List of Buttons"))
	{
		for (size_t i = 0; i < _listButtons.size() + 1; i++)
		{
			if (i != _listButtons.size())
			{
				ImGui::Text("   - ");
				ImGui::SameLine();
				ImGui::Button(_listButtons[i]->GetGameObject()->GetName().c_str());
			}
			else
			{
				ImGui::Text("   - ");
				ImGui::SameLine();
				ImGui::Button("Insert a button");
			}
			
			//DragBox Button
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//Drop asset from Asset window to scene window
					const uint* drop = (uint*)payload->Data;

					ComponentUIButton* UIB = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUIButton>();

					if (UIB != nullptr)
					{
						if (i != _listButtons.size())
						{
							_listButtons[i] = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUIButton>();
						}
						else
						{
							_listButtons.push_back(UIB);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			//DragBox CheckBox
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//Drop asset from Asset window to scene window
					const uint* drop = (uint*)payload->Data;

					ComponentUICheckbox* UIB = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUICheckbox>();

					if (UIB != nullptr)
					{
						if (i != _listButtons.size())
						{
							_listButtons[i] = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUICheckbox>();
						}
						else
						{
							_listButtons.push_back(UIB);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			//DragBox Slider
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					//Drop asset from Asset window to scene window
					const uint* drop = (uint*)payload->Data;

					ComponentUISlider* UIB = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUISlider>();

					if (UIB != nullptr)
					{
						if (i != _listButtons.size())
						{
							_listButtons[i] = ModuleLayers::gameObjects[*drop]->GetComponent<ComponentUISlider>();
						}
						else
						{
							_listButtons.push_back(UIB);
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::Text("");

		if (ImGui::Button("Reset List"))
		{
			_listButtons.clear();
		}
	}

}
#endif // STANDALONE