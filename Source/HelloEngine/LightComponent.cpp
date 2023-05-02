#include "Headers.h"
#include "LightComponent.h"
#include "GameObject.h"

#include "Lighting.h"

LightComponent::LightComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::LIGHT;
	_name = "Light";

	_needsTransformCallback = true;
}

LightComponent::~LightComponent()
{
	Lighting::RemoveLight(_type, _lightID);
}

void LightComponent::OnEnable()
{
	data.isEnabled = true;
	UpdateToLightMap();
}

void LightComponent::OnDisable()
{
	data.isEnabled = false;
	UpdateToLightMap();
}

void LightComponent::Serialization(json& _j)
{
	json j;

	j["Type"] = _type;

	j["Color"] = { data.color.x, data.color.y, data.color.z };

	j["Ambient Intensity"] = data.ambientIntensity;
	j["Diffuse Intensity"] = data.diffuseIntensity;

	SerializationUnique(j);

	j["Enabled"] = _isEnabled;

	_j["Components"].push_back(j);
}

void LightComponent::SerializationUnique(json& j)
{
}

void LightComponent::DeSerialization(json& _j)
{
	std::vector<float> temp = _j["Color"];
	data.color = { temp[0], temp[1], temp[2] };

	data.ambientIntensity = _j["Ambient Intensity"];
	data.diffuseIntensity = _j["Diffuse Intensity"];

	bool enabled = _j["Enabled"];
	if (!enabled)Disable();

	DeSerializationUnique(_j);
}

void LightComponent::DeSerializationUnique(json& j)
{

}

// I know this is a crime against humanity but it was the best temporal solution I could think of :<
void LightComponent::UpdateData(Light& data)
{
	data.ambientIntensity = this->data.ambientIntensity;
	data.color = this->data.color;
	data.diffuseIntensity = this->data.diffuseIntensity;
	data.isEnabled = this->data.isEnabled;
}

void LightComponent::UpdateToLightMap()
{

}

#ifdef STANDALONE
void LightComponent::OnEditor()
{
	bool created = true;

	if (ImGui::CollapsingHeader(_name.c_str(), &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Light Parameters");
		ImGui::Separator();

		//COLOUR
		std::string popName = "##LightColorPicker";
		if (ImGui::ColorButton("Light Colour", (ImVec4&)data.color, 0, { 50, 50 }))
		{
			ImGui::OpenPopup(popName.c_str());
		}
		ImGui::SameLine();
		ImGui::Text("Light Colour");

		if (ImGui::BeginPopup(popName.c_str()))
		{
			ImGui::ColorPicker3("Light Colour", &data.color.At(0), 0 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			ImGui::EndPopup();
		}

		//Ambient intensity
		ImGui::DragFloat("Ambient Intensity", &data.ambientIntensity, 0.25f, 0.25f, 1000.0f);

		//Diffuse intensity
		ImGui::DragFloat("Diffuse Intensity", &data.diffuseIntensity, 0.25f, 0.25f, 1000.0f);

		//Unique per child
		ImGui::NewLine();
		switch (_type)
		{
			case Type::DIRECTIONAL_LIGHT: ImGui::Text("Directional Parameters"); break;
			case Type::POINT_LIGHT: ImGui::Text("Point Parameters"); break;
			case Type::SPOT_LIGHT: ImGui::Text("Spot Parameters"); break;
		}
		ImGui::Separator();
		OnEditorUnique();

		ImGui::NewLine();
	}

	//Application::Instance()->renderer3D->renderManager._lightMap[_lightID] = data;

	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void LightComponent::OnEditorUnique()
{

}

void LightComponent::MarkAsDead()
{
	Lighting::RemoveLight(_type, _lightID);
}


#endif
