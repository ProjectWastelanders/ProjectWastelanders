#include "Headers.h"
#include "P_SphereShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_SphereShape::P_SphereShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::SPHERE;

	radius = 10;
}

P_SphereShape::~P_SphereShape()
{
}

void P_SphereShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Sphere Shape Emitter"))
		ImGui::DragFloat("Radius", &radius);
}

float3 P_SphereShape::GetRandomPos()
{
	LCG random;
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float angle = random.Float() * (2 * math::pi);
	float r = random.Float() * radius;
	float p = random.Float() * math::pi;

	float x = r * sin(p) * cos(angle);
	float y = r * sin(p) * sin(angle);
	float z = r * cos(p);

	x += center.x;
	y += center.y;
	z += center.z;

	return float3(x, y, z);
}

bool P_SphereShape::IsInside(float3 position)
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float distance = (position - component->GetGameObject()->transform->GetGlobalPosition()).Normalize();

	return distance <= radius;
}

