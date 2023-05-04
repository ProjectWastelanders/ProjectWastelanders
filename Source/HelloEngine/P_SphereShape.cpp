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

	float x, y, z;

	while (true) {
		x = random.Float(-radius, radius);
		y = random.Float(-radius, radius);
		z = random.Float(-radius, radius);

		if (sqrt((x * x) + (y * y) + (z * z)) < radius) break;
	}

	x += center.x;
	y += center.y;
	z += center.z;

	return float3(x, y, z);
}

bool P_SphereShape::IsInside(float3 position)
{
	/*center = component->GetGameObject()->transform->GetGlobalPosition();

	float distance = (position - component->GetGameObject()->transform->GetGlobalPosition()).Normalize();

	return distance <= radius;*/
	return true;
}

