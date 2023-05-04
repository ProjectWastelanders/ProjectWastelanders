#include "Headers.h"
#include "P_ConeShape.h"
#include "ParticleSystemComponent.h"
#include "GameObject.h"

P_ConeShape::P_ConeShape()
{
	type = P_ModuleType::SHAPE;
	shapeType = ShapeType::CONE;

	radius = 10;
	height = 20;
	
}

P_ConeShape::~P_ConeShape()
{
}

void P_ConeShape::OnEditor()
{
	if (ImGui::CollapsingHeader("Cube Shape Emitter"))
	{
		ImGui::DragFloat("Radius", &radius);
		ImGui::DragFloat("Height", &height);
	}
}

float3 P_ConeShape::GetRandomPos()
{
	center = component->GetGameObject()->transform->GetGlobalPosition();

	LCG random;

	float h = height * random.Float(0.0f, 1.0f);
	float r = (radius / height) * h * sqrt(random.Float(0.0f, 1.0f));
	float angle = 2 * math::pi * random.Float(0.0f, 1.0f);


	float x = r * cos(angle);
	float y = h;
	float z = r * sin(angle);
	
	x += center.x;
	y += center.y;
	z += center.z;

	return float3(x, y, z);
}

bool P_ConeShape::IsInside(float3 position)
{
	//center = component->GetGameObject()->transform->GetGlobalPosition();

	/*float limitX = center.x + c1;
	float limitY = center.x + c1;
	float limitZ = center.z + c2;

	if (position.x <= limitX && position.y <= limitY && position.z <= limitZ)
	{
		return true;
	}*/

	return true;
}