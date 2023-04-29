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
	Lighting::SetDirectionalLight(this->data);

	CalculateLightSpaceMatrix();
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
	UpdateToLightMap();
	//ImGui::DragFloat3("Direction", &data.direction.At(0), 0.05f, -1.0f, 1.0f);
}

void DirectionalLightComponent::MarkAsAlive()
{
	UpdateToLightMap();
}
#endif

void DirectionalLightComponent::CalculateLightSpaceMatrix()
{
	lightFrustum.type = math::FrustumType::OrthographicFrustum;
	lightFrustum.nearPlaneDistance = 0.1f;
	lightFrustum.farPlaneDistance = 1000.0f;

	//Look At
	lightFrustum.front = (float3(0.0f, 0.0f, 0.0f) - lightFrustum.pos).Normalized();
	float3 X = float3(0, 1, 0).Cross(lightFrustum.front).Normalized();
	lightFrustum.up = lightFrustum.front.Cross(X);

	data.lightSpaceMatrix = lightFrustum.ProjectionMatrix().Transposed();
}