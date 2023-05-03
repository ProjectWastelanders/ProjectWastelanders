#include "Headers.h"
#include "P_CircleShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_CircleShape::P_CircleShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::CIRCLE;
}

P_CircleShape::~P_CircleShape()
{
}

void P_CircleShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Circle Shape Emitter"))
	ImGui::DragFloat("Radius", &radius);
}

float3 P_CircleShape::GetRandomPos()
{
	LCG random;
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float angle = random.Float() * (2 * math::pi);
	float r = random.Float() * radius;

	float x = r * cos(angle);
	float y = 0;
	float z = r * sin(angle);

	x += center.x;
	z += center.z;

	return float3(x, y, z);
}

bool P_CircleShape::IsInside(float3 position)
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float distance = (position - component->GetGameObject()->transform->GetGlobalPosition()).Normalize();

	return distance <= radius;
}
