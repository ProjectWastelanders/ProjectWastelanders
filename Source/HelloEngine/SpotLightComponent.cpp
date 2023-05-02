#include "Headers.h"
#include "SpotLightComponent.h"

#include "GameObject.h"
#include "Lighting.h"

SpotLightComponent::SpotLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::SPOT_LIGHT;
	_name = "Spot Light";

	data = SpotLight();

	_lightID = Lighting::AddSpotLight(this->data);
}

SpotLightComponent::~SpotLightComponent()
{

}

void SpotLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	data.position = worldMatrix.TranslatePart();
	data.direction = _gameObject->GetComponent<TransformComponent>()->GetLocalRotation();

	UpdateToLightMap();
}

void SpotLightComponent::UpdateToLightMap()
{
	UpdateData(this->data);
	Lighting::GetLightMap().spotLight[_lightID] = this->data;
}

void SpotLightComponent::SerializationUnique(json& j)
{
	j["Cutoff"] = data.cutoff;
	j["Distance"] = data.distance;


	j["Linear"] = data.linear;
	j["Exp"] = data.exp;
}

void SpotLightComponent::DeSerializationUnique(json& j)
{
	//UpdateData(this->data);

	data.cutoff = j["Cutoff"];
	data.distance = j["Distance"];

	data.linear = j["Linear"];
	data.exp = j["Exp"];
	
	float4x4 transform = _gameObject->GetComponent<TransformComponent>()->GetGlobalMatrix();
	data.position = transform.TranslatePart();
	data.direction = _gameObject->GetComponent<TransformComponent>()->GetLocalRotation();

	UpdateToLightMap();
}

#ifdef STANDALONE
void SpotLightComponent::OnEditorUnique()
{
	ImGui::DragFloat("Cutoff", &data.cutoff, 0.5f, 0.05f, 179.5f);
	ImGui::DragFloat("Distance", &data.distance, 0.5f, 0.5f, 1000.0f);

	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation");
	//ImGui::DragFloat("Constant", &data.constant);
	ImGui::DragFloat("Linear", &data.linear, 0.05f, 0.05f, 0.0f);
	ImGui::DragFloat("Quadratic", &data.exp, 0.05f, 0.0f, 1.0f);

	UpdateToLightMap();
}

void SpotLightComponent::MarkAsAlive()
{
	UpdateToLightMap();
}
#endif