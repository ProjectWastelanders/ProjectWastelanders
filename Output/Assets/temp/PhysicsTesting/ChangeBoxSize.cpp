#include "ChangeBoxSize.h"

HELLO_ENGINE_API_C ChangeBoxSize* CreateChangeBoxSize(ScriptToInspectorInterface* script)
{
	ChangeBoxSize* classInstance = new ChangeBoxSize();
	//Show variables inside the inspector using script->AddDragInt("variableName", &classInstance->variable);
	return classInstance;
}

void ChangeBoxSize::Start()
{
	API_RigidBody boxCol = gameObject.GetRigidBody();
	API_Vector3 boxColScl = boxCol.GetBoxScale();
	API_Vector3 a(2, 10, 2);
	
	if (boxColScl == a) 
	{
		boxCol.SetBoxScale(API_Vector3(10,10,10));
	}

	float SphereColScl = boxCol.GetRadius();
	float b(10);

	if (SphereColScl == b)
	{
		boxCol.SetRadius(20);
	}

	API_Vector2 CylColScl = boxCol.GetCylinderScale();
	API_Vector2 c(10, 6);

	if (CylColScl == c)
	{
		boxCol.SetCylinderScale(20, 12);
	}
	
}

void ChangeBoxSize::Update()
{

}