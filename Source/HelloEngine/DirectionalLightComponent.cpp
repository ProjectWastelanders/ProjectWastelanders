#include "Headers.h"
#include "DirectionalLightComponent.h"

#include "GameObject.h"
#include "Lighting.h"

DirectionalLightComponent::DirectionalLightComponent(GameObject* gameObject) : LightComponent(gameObject)
{
	_type = Type::DIRECTIONAL_LIGHT;
	_name = "Directional Light";

	data = DirectionalLight();

	Lighting::SetDirectionalLight(this->data);

	data.lightFrustum.type = math::FrustumType::OrthographicFrustum;
	data.lightFrustum.nearPlaneDistance = 0.0f;
	data.lightFrustum.farPlaneDistance = 185;
	data.lightFrustum.orthographicHeight = 64;
	data.lightFrustum.orthographicWidth = 64;

}

DirectionalLightComponent::~DirectionalLightComponent()
{
	Lighting::RemoveLight(data.GetType(), _lightID);
}

void DirectionalLightComponent::OnTransformCallback(float4x4 worldMatrix)
{
	//data.direction = worldMatrix.ToEulerXYZ();
	data.direction = _gameObject->GetComponent<TransformComponent>()->GetLocalRotation();

	UpdateToLightMap();
}

void DirectionalLightComponent::UpdateToLightMap()
{
	UpdateData(this->data);
	data.CalculateLightSpaceMatrix();

	//Always last
	Lighting::SetDirectionalLight(this->data);
}

void DirectionalLightComponent::SerializationUnique(json& j)
{
	//j["Direction"] = { data.direction.x, data.direction.y, data.direction.z };
}

void DirectionalLightComponent::DeSerializationUnique(json& j)
{
	/*std::vector<float> temp = j["Direction"];
	data.direction = { temp[0], temp[1], temp[2] };*/

	//data.direction = _gameObject->GetComponent<TransformComponent>()->GetGlobalMatrix().ToEulerXYZ();
	data.direction = _gameObject->GetComponent<TransformComponent>()->GetLocalRotation();

	UpdateToLightMap();
}

#ifdef STANDALONE
void DirectionalLightComponent::OnEditorUnique()
{
	//ImGui::DragFloat("Near plane Distance", &data.lightFrustum.nearPlaneDistance);
	//ImGui::DragFloat("Far plane Distance", &data.lightFrustum.farPlaneDistance);
	//ImGui::DragFloat("Orthographic Height", &data.lightFrustum.orthographicHeight);
	//ImGui::DragFloat("Orthographic Width", &data.lightFrustum.orthographicWidth);

	//UpdateToLightMap();
}

void DirectionalLightComponent::MarkAsAlive()
{
	UpdateToLightMap();
}
#endif

void DirectionalLight::CalculateLightSpaceMatrix()
{
	lightFrustum.front = (-direction).Normalized();
	float3 X = float3(0, 1, 0).Cross(lightFrustum.front).Normalized();
	lightFrustum.up = lightFrustum.front.Cross(X);

	lightSpaceMatrix = lightFrustum.ViewProjMatrix().Transposed();
}