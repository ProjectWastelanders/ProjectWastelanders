#include "Headers.h"
#include "PointLightComponent.h"

#include "GameObject.h"
#include "Lighting.h"

PointLightComponent::PointLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::POINT_LIGHT;
	_name = "Point Light";

	data = PointLight();
	data = this->data;

	_lightID = Lighting::AddPointLight(this->data);
}

PointLightComponent::~PointLightComponent()
{

}

void PointLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	data.position = worldMatrix.TranslatePart();

	UpdateToLightMap();
}

void PointLightComponent::UpdateToLightMap()
{
	UpdateData(this->data);
	Lighting::GetLightMap().pointLight[_lightID] = this->data;
}

void PointLightComponent::SerializationUnique(json& j)
{
	j["Linear"] = data.linear;
	j["Exp"] = data.exp;

	j["Distance"] = data.distance;
}

void PointLightComponent::DeSerializationUnique(json& j)
{
	data.linear = j["Linear"];
	data.exp = j["Exp"];

	data.distance = j["Distance"];

	data.position = _gameObject->GetComponent<TransformComponent>()->GetGlobalMatrix().TranslatePart();

	UpdateToLightMap();
}

#ifdef STANDALONE
void PointLightComponent::OnEditorUnique()
{
	ImGui::DragFloat("Distance", &data.distance, 0.5f, 0.5f, 1000.0f);

	//UpdateData(this->data);
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation");
	//ImGui::DragFloat("Constant", &data.constant);
	ImGui::DragFloat("Linear", &data.linear, 0.05f, 0.05f, 0.0f);
	ImGui::DragFloat("Quadratic", &data.exp, 0.05f, 0.0f, 1.0f);
	
	UpdateToLightMap();
}

void PointLightComponent::MarkAsAlive()
{
	UpdateToLightMap();
}
#endif