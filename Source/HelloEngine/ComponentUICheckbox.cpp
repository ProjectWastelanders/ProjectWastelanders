#include "Headers.h"
#include "ComponentUICheckbox.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "ComponentUIButton.h"
#include "ComponentUISlider.h"
#include "API/API_UICheckBox.h"

ComponentUICheckbox::ComponentUICheckbox(GameObject* gameObject) : ComponentUI(gameObject)
{
	State = CheckboxState::NORMAL;
	_type = Component::Type::UI_CHECKBOX;
}

ComponentUICheckbox::~ComponentUICheckbox()
{
}

void ComponentUICheckbox::InputUpdate()
{
	// Add here any checks necessary with INPUT.

	// PROBLEMA A SOLUCIONAR: NO PODEM TENIR ACTIVE I HOVERED AL MATEI TEMPS (EN PROCES DE PENSAR UNA SOLUCIÓ) 

	switch (State)
	{
	case CheckboxState::NORMAL:
		_material->ChangeTexture(textureIDIdleCB);
		break;
	case CheckboxState::HOVERED:
		_material->ChangeTexture(textureIDHoverCB);
		break;
	case CheckboxState::ONPRESS:
		_material->ChangeTexture(textureIDPressCB);
		break;
	case CheckboxState::NORMALACTIVE:
		_material->ChangeTexture(textureIDActiveCB);
		break;
	case CheckboxState::HOVEREDACTIVE:
		_material->ChangeTexture(textureIDHoverActiveCB);
		break;
	case CheckboxState::ONPRESSACTIVE:
		_material->ChangeTexture(textureIDPressActiveCB);
		break;
	default:
		break;
	}
}

void ComponentUICheckbox::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	_j["State"] = State;
	_j["idleImage"] = idleCB ? idleCB->UID : 0;
	_j["hoverImage"] = hoverCB ? hoverCB->UID : 0;
	_j["pressImage"] = pressCB ? pressCB->UID : 0;
	_j["idleActiveImage"] = activeCB ? activeCB->UID : 0;
	_j["hoverActiveImage"] = hoverActiveCB ? hoverActiveCB->UID : 0;
	_j["pressActiveImage"] = pressActiveCB ? pressActiveCB->UID : 0;
	SaveMeshState(_j);
	//_j["checkActive"] = checkActive;
	j["Components"].push_back(_j);
}

void ComponentUICheckbox::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_gameObject->transform->ForceUpdate();

	uint savedUIDIdle = j["idleImage"];
	idleCB = savedUIDIdle == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["idleImage"]);
	if (idleCB != nullptr)
		textureIDIdleCB = idleCB->OpenGLID;
	else
		textureIDIdleCB = -1;

	uint savedUIDHover = j["hoverImage"];
	hoverCB = savedUIDHover == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["hoverImage"]);
	if (hoverCB != nullptr)
		textureIDHoverCB = hoverCB->OpenGLID;
	else
		textureIDHoverCB = -1;

	uint savedUIDPress = j["pressImage"];
	pressCB = savedUIDPress == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["pressImage"]);
	if (pressCB != nullptr)
		textureIDPressCB = pressCB->OpenGLID;
	else
		textureIDPressCB = -1;

	uint savedUIDIdleActive = j["idleActiveImage"];
	activeCB = savedUIDIdleActive == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["idleActiveImage"]);
	if (activeCB != nullptr)
		textureIDActiveCB = activeCB->OpenGLID;
	else
		textureIDActiveCB = -1;

	uint savedUIDHoverActive = j["hoverActiveImage"];
	hoverActiveCB = savedUIDHoverActive == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["hoverActiveImage"]);
	if (hoverActiveCB != nullptr)
		textureIDHoverActiveCB = hoverActiveCB->OpenGLID;
	else
		textureIDHoverActiveCB = -1;

	uint savedUIDPressActive = j["pressActiveImage"];
	pressActiveCB = savedUIDPressActive == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["pressActiveImage"]);
	if (pressActiveCB != nullptr)
		textureIDPressActiveCB = pressActiveCB->OpenGLID;
	else
		textureIDPressActiveCB = -1;

	State = j["State"];
	//checkActive = j["checkActive"];
	LoadMeshState(j);

	switch (State)
	{
	case CheckboxState::NORMAL:
		_material->ChangeTexture(idleCB);
		break;
	case CheckboxState::HOVERED:
		_material->ChangeTexture(hoverCB);
		break;
	case CheckboxState::ONPRESS:
		_material->ChangeTexture(pressCB);
		break;
	case CheckboxState::NORMALACTIVE:
		_material->ChangeTexture(activeCB);
		break;
	case CheckboxState::HOVEREDACTIVE:
		_material->ChangeTexture(hoverActiveCB);
		break;
	case CheckboxState::ONPRESSACTIVE:
		_material->ChangeTexture(pressActiveCB);
		break;
	default:
		break;
	}
}

CheckboxState ComponentUICheckbox::ChangeState(CheckboxState State)
{
	if (IsMouseOver()) {

		if (ModuleInput::S_GetMouseButton(1) != KEY_DOWN && State != CheckboxState::HOVEREDACTIVE && State != CheckboxState::NORMALACTIVE)
		{
			_material->ChangeTexture(textureIDHoverCB);
			State = CheckboxState::HOVERED;
		}

		if (ModuleInput::S_GetMouseButton(1) == KEY_UP)
		{
			State = CheckboxState::ONPRESS;

			if (checkActive == false)
			{
				checkActive = true;
			}

			else if (checkActive == true)
			{
				checkActive = false;
			}

		}

		if (State == CheckboxState::NORMALACTIVE)
		{
			_material->ChangeTexture(textureIDHoverCB);
			State = CheckboxState::HOVEREDACTIVE;
		}
	}

	if (State == CheckboxState::ONPRESS)
	{
		if (checkActive == true)
			State = CheckboxState::NORMALACTIVE;

		else if (checkActive == false)
			State = CheckboxState::NORMAL;
	}

	if (!IsMouseOver())
	{
		if (State != CheckboxState::HOVEREDACTIVE && State != CheckboxState::NORMALACTIVE)
		{
			_material->ChangeTexture(textureIDIdleCB);
			State = CheckboxState::NORMAL;
		}

		if (State == CheckboxState::HOVEREDACTIVE)
		{
			_material->ChangeTexture(textureIDPressCB);
			State = CheckboxState::NORMALACTIVE;
		}
	}

	return State;
}

void ComponentUICheckbox::UpdateGamePadInput(bool selected)
{
	if (selected)
	{
		if (ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_DOWN && (State != CheckboxState::ONPRESS && State != CheckboxState::ONPRESSACTIVE))
		{
			ModuleInput::S_HandleGamePadButton(GamePad::BUTTON_A); // Handle A button so no other UI detects it.
			if (checkActive)
			{
				State = CheckboxState::ONPRESSACTIVE;
			}
			else
			{
				State = CheckboxState::ONPRESS;
			}
			//isPress = true;
		}
		else
		{
			// If previous state was hold or press, this frame the button was released
			if (ModuleInput::S_GetGamePadButton(GamePad::BUTTON_A) == KEY_UP && (State == CheckboxState::ONPRESS || State == CheckboxState::ONPRESSACTIVE))
			{
				//isReleased = true;
			}
			if (checkActive)
			{
				State = CheckboxState::HOVEREDACTIVE;
			}
			else
			{
				State = CheckboxState::HOVERED;
			}
		}
	}
	else
	{
		if (checkActive)
		{
			State = CheckboxState::NORMALACTIVE;
		}
		else
		{
			State = CheckboxState::NORMAL;
		}
	}


	switch (State)
	{
	case CheckboxState::NORMAL:
		_material->ChangeTexture(textureIDIdleCB);
		break;
	case CheckboxState::HOVERED:
		_material->ChangeTexture(textureIDHoverCB);
		break;
	case CheckboxState::ONPRESS:
		_material->ChangeTexture(textureIDPressCB);
		break;
	case CheckboxState::NORMALACTIVE:
		_material->ChangeTexture(textureIDActiveCB);
		break;
	case CheckboxState::HOVEREDACTIVE:
		_material->ChangeTexture(textureIDHoverActiveCB);
		break;
	case CheckboxState::ONPRESSACTIVE:
		_material->ChangeTexture(textureIDPressActiveCB);
		break;
	default:
		break;
	}
}

#ifdef STANDALONE
void ComponentUICheckbox::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Check Box", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	/*bool auxiliaryBool = _isEnabled;
	if (ImGui::Checkbox("Active##Material", &auxiliaryBool))
		auxiliaryBool ? Enable() : Disable();*/

	ImGui::Text("Im a CHECK BOX");
	ImGui::Text("States Colors:");

	ImGui::Text("NORMAL"); ImGui::SameLine();
	ImGui::ColorEdit4("color", colors);
	//ImGui::Text("HOVERED");
	//ImGui::SameLine();
	//ImGui::ColorEdit3("color", colors);
	//ImGui::Text("ONPRESS");
	//ImGui::SameLine();
	//ImGui::ColorEdit3("color", colors);
	//ImGui::Text("ONHOLD");
	//ImGui::SameLine();
	//ImGui::ColorEdit3("color", colors);
	ImGui::Separator();
	ImGui::Text("States Textures:");
	ImGui::Text("Normal:"); ImGui::SameLine();

	{
		std::string imageName;
		int width = 0;
		int height = 0;

		if (textureIDIdleCB != -1.0f && idleCB != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureIDIdleCB, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

			imageName = idleCB->debugName;
			width = idleCB->width;
			height = idleCB->height;
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				idleCB = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
				textureIDIdleCB = idleCB->OpenGLID;
				_material->ChangeTexture(idleCB);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("Hovered:"); ImGui::SameLine();
	{
		//Mesh& mesh = _material->GetMesh();

		std::string imageName;
		int width = 0;
		int height = 0;

		if (textureIDHoverCB != -1.0f && hoverCB != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureIDHoverCB, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

			imageName = hoverCB->debugName;
			width = hoverCB->width;
			height = hoverCB->height;
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				hoverCB = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
				textureIDHoverCB = hoverCB->OpenGLID;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("Press:"); ImGui::SameLine();
	{
		//Mesh& mesh = _material->GetMesh();

		std::string imageName;
		int width = 0;
		int height = 0;

		if (textureIDPressCB != -1.0f && pressCB != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureIDPressCB, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

			imageName = pressCB->debugName;
			width = pressCB->width;
			height = pressCB->height;
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				pressCB = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
				textureIDPressCB = pressCB->OpenGLID;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("Normal Active:"); ImGui::SameLine();
	{
		//Mesh& mesh = _material->GetMesh();

		std::string imageName;
		int width = 0;
		int height = 0;

		if (textureIDActiveCB != -1.0f && activeCB != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureIDActiveCB, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

			imageName = activeCB->debugName;
			width = activeCB->width;
			height = activeCB->height;
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				activeCB = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
				textureIDActiveCB = activeCB->OpenGLID;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("Hover Active:"); ImGui::SameLine();
	{
		//Mesh& mesh = _material->GetMesh();

		std::string imageName;
		int width = 0;
		int height = 0;

		if (textureIDHoverActiveCB != -1.0f && hoverActiveCB != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureIDHoverActiveCB, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

			imageName = hoverActiveCB->debugName;
			width = hoverActiveCB->width;
			height = hoverActiveCB->height;
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				hoverActiveCB = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
				textureIDHoverActiveCB = hoverActiveCB->OpenGLID;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}

	//////////////////////////////////////////////////////////////////////////////////////
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("Press Active:"); ImGui::SameLine();
	{
		//Mesh& mesh = _material->GetMesh();

		std::string imageName;
		int width = 0;
		int height = 0;

		if (textureIDPressActiveCB != -1.0f && pressActiveCB != nullptr)
		{
			ImGui::Image((ImTextureID)(uint)textureIDPressActiveCB, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

			imageName = pressActiveCB->debugName;
			width = pressActiveCB->width;
			height = pressActiveCB->height;
		}
		else
		{
			ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
			imageName = "None";
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				pressActiveCB = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);
				textureIDPressActiveCB = pressActiveCB->OpenGLID;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::TextWrapped("Path: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), imageName.c_str());

		ImGui::TextWrapped("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(width).c_str());

		ImGui::TextWrapped("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(height).c_str());
	}
}
#endif // STANDALONE