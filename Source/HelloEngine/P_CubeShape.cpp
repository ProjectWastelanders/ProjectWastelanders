#include "Headers.h"
#include "P_CubeShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_CubeShape::P_CubeShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::CUBE;

	c1 = 10;
	c2 = 10;
	c3 = 10;
}

P_CubeShape::~P_CubeShape()
{
}

void P_CubeShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Cube Shape Emitter"))
	{
		ImGui::DragFloat("Side X", &c1);
		ImGui::DragFloat("Side Y", &c2);
		ImGui::DragFloat("Side Z", &c3);
	}
}

float3 P_CubeShape::GetRandomPos()
{
	center = component->GetGameObject()->transform->GetGlobalPosition();
	LCG random;

	float x = random.Float() * c1;
	float y = random.Float() * c2;
	float z = random.Float() * c3;

	x += center.x;
	y += center.y;
	z += center.z;

	return float3(x, y, z);
}

bool P_CubeShape::IsInside(float3 position)
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	float limitX = center.x + c1;
	float limitY = center.x + c1;
	float limitZ = center.z + c2;

	if (position.x <= limitX && position.y <= limitY && position.z <= limitZ)
	{
		return true;
	}

	return false;
}