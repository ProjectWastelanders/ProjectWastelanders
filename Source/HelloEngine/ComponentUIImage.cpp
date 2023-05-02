#include "Headers.h"
#include "ComponentUIImage.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "ImWindowGame.h"
#include "LayerEditor.h"

ComponentUIImage::ComponentUIImage(GameObject* gameObject) : ComponentUI(gameObject)
{
	_type = Component::Type::UI_IMAGE;

	_fillImage = 1.f;
	_maxScale = 1.f;
	_gameWindow = (ImWindowGame*)LayerEditor::_imWindows[(uint)ImWindowID::GAME];
}

ComponentUIImage::~ComponentUIImage()
{
}

void ComponentUIImage::InputUpdate()
{

	//if()

}

void ComponentUIImage::SetFill(float fill)
{
	_meshRenderer->GetMesh().opacityLimit = fill;
}

void ComponentUIImage::SetOpacity(float opacity)
{
	_meshRenderer->GetMesh().opacity = opacity;
}

void ComponentUIImage::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["MaterialResource"] = _material->GetResourceUID();
	_j["Enabled"] = _isEnabled;
	_j["FillImage"] = _meshRenderer->GetMesh().opacityLimit;
	_j["OpacityDirection"] = (uint)_meshRenderer->GetMesh().opacityDir;
	SaveMeshState(_j);
	j["Components"].push_back(_j);
}

void ComponentUIImage::DeSerialization(json& j)
{
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(j["MaterialResource"]));

	_meshRenderer->GetMesh().opacityLimit = j["FillImage"];

	if (j.contains("OpacityDirection"))
	{
		_meshRenderer->GetMesh().opacityDir = (OpacityDirection)(int)j["OpacityDirection"];
	}

	LoadMeshState(j);

	_gameObject->transform->ForceUpdate();
	
	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}

#ifdef STANDALONE
void ComponentUIImage::OnEditor()
{
	bool created = true;
	if (!ImGui::CollapsingHeader("Image", &created, ImGuiTreeNodeFlags_DefaultOpen)) return;
	if (!created)
	{
		_gameObject->DestroyComponent(this);
		return;
	}

	ImGui::SliderFloat("Opacity", &_meshRenderer->GetMesh().opacity, 0.0f, 1.0f);

	ImGui::SliderFloat("Fill", &_meshRenderer->GetMesh().opacityLimit, 0.0f, 1.0f);

	uint currentOpacityDir = (uint)_meshRenderer->GetMesh().opacityDir;

	if (ImGui::BeginCombo("Opacity Direction", comboNames[currentOpacityDir].c_str()))
	{
		for (int i = 0; i < 4; ++i)
		{
			bool selected = i == currentOpacityDir;
			if (ImGui::Selectable(comboNames[i].c_str(), &selected))
			{
				_meshRenderer->GetMesh().opacityDir = (OpacityDirection)i;
			}
		}
		ImGui::EndCombo();
	}


	ImGui::InputFloat("Max Scale", &_maxScale);
	

}
#endif // STANDALONE