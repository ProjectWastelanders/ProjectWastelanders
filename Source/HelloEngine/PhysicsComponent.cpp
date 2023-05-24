#include "Headers.h"
#include "PhysicsComponent.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"
#include "ModuleLayers.h"

PhysicsComponent::PhysicsComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Component::Type::PHYSICS;
	_physBody = nullptr;
	_needsTransformCallback = true;

	_shapeSelected = ColliderShape::NONE;

	_isShapeSelected[0] = false;
	_isShapeSelected[1] = false;
	_isShapeSelected[2] = false;

	_isShapeCreated[0] = false;
	_isShapeCreated[1] = false;
	_isShapeCreated[2] = false;

	sphereRadius = 1;
	cylRadiusHeight = { 1,1 };

	renderColColor[0] = 0.5f;
	renderColColor[1] = 0.0f;
	renderColColor[2] = 0.5f;
	renderColColor[3] = 1.0f;

	wireframeSize = 1.0f;

	raycastColor[0] = 0.5f;
	raycastColor[1] = 0.0f;
	raycastColor[2] = 0.5f;
	raycastColor[3] = 1.0f;

	raycastSize = 1.0f;

	sphereVerSlices = 16;
	sphereHorSlices = 16;
	cylinderVerSlices = 16;

	_gravity[0] = { 0 };
	_gravity[1] = { -9.8 };
	_gravity[2] = { 0 };

	_angularFactor[0] = 1.0f;
	_angularFactor[1] = 1.0f;
	_angularFactor[2] = 1.0f;
}

PhysicsComponent::~PhysicsComponent()
{
	if (_physBody)
		RemoveCollider();
	ModuleLayers::RemoveRigidBody(_gameObject->GetID());
}

void PhysicsComponent::SetVelocity(float3 vec)
{
	if (!_physBody)
		return;

	_physBody->SetVelocity(vec.x, vec.y, vec.z);
}

float3 PhysicsComponent::GetVelocity()
{
	return _physBody->GetVelocity();
}

void PhysicsComponent::SetGravity(float3 grav)
{
	_physBody->SetGravity(float3(grav[0], grav[1], grav[2]));
	_gravity[0] = grav[0];
	_gravity[1] = grav[1];
	_gravity[2] = grav[2];
}

float3 PhysicsComponent::GetGravity()
{
	return float3(_gravity[0], _gravity[1], _gravity[2]);
}

void PhysicsComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	_j["Enabled"] = _isEnabled;

	if (_physBody)
	{
		_j["HasPhysBody"] = true;
		_j["IsRenderingCol"] = _physBody->isRenderingCol;
		_j["IsRenderingRayCast"] = _physBody->isRenderingRayCast;
	}
	else
	{
		_j["HasPhysBody"] = false;
		_j["IsRenderingCol"] = false;
		_j["IsRenderingRayCast"] = false;
	}

	_j["ShapeSelected"] = _shapeSelected;

	_j["IsShapeSelected"] = { _isShapeSelected[0], _isShapeSelected[1], _isShapeSelected[2] };
	_j["IsShapeCreated"] = { _isShapeCreated[0], _isShapeCreated[1], _isShapeCreated[2] };

	if (_physBody)
	{
		_j["Mass"] = _physBody->mass;

		_j["ColPosition"] = { _physBody->colPos[0], _physBody->colPos[1], _physBody->colPos[2] };
		_j["ColRotation"] = { _physBody->colRot[0], _physBody->colRot[1],_physBody->colRot[2] };
		_j["ColScale"] = { _physBody->colScl[0], _physBody->colScl[1], _physBody->colScl[2] };

		_j["IsStatic"] = _physBody->isStatic;
		_j["IsKinematic"] = _physBody->isKinematic;
		_j["IsTrigger"] = _physBody->isTrigger;

		_j["Gravity"] = { _gravity[0], _gravity[1], _gravity[2] };
		_j["AngularFactor"] = { _angularFactor[0], _angularFactor[1], _angularFactor[2] };
		_j["WireframeSize"] = wireframeSize;

		_j["SphereVerSlices"] = sphereVerSlices;
		_j["SphereHorSlices"] = sphereHorSlices;
		_j["CylVerSlices"] = cylinderVerSlices;

		_j["RenderColColor"] = { renderColColor[0] , renderColColor[1] , renderColColor[2], renderColColor[3] };

		_j["RayCastSize"] = raycastSize;
		_j["RayCastColor"] = { raycastColor[0] , raycastColor[1] , raycastColor[2], raycastColor[3] };
	}
	else
	{
		_j["Mass"] = 1.0f;

		_j["ColPosition"] = { 0, 0, 0 };
		_j["ColRotation"] = { 0, 0, 0 };
		_j["ColScale"] = { 0, 0, 0 };

		_j["IsStatic"] = false;
		_j["IsKinematic"] = false;
		_j["IsTrigger"] = false;

		_j["Gravity"] = { 0, -9.8f, 0 };

		_j["WireframeSize"] = 1.0f;

		_j["SphereVerSlices"] = 16;
		_j["SphereHorSlices"] = 16;
		_j["CylVerSlices"] = 16;

		_j["RenderColColor"] = { 0.5f , 0 , 0.5f, 1 };

		_j["RayCastSize"] = 1.0f;
		_j["RayCastColor"] = { 0.5f , 0 , 0.5f, 1 };
	}

	_j["SphereRadius"] = sphereRadius;

	_j["CylinderRadiusHeight"] = { cylRadiusHeight[0], cylRadiusHeight[1] };

	j["Components"].push_back(_j);
}

void PhysicsComponent::DeSerialization(json& j)
{
	bool enabled = j["Enabled"];

	if (!enabled)
		Disable();

	_shapeSelected = j["ShapeSelected"];

	std::vector<bool> isShapeSelectedTemp = j["IsShapeSelected"];
	_isShapeSelected[0] = isShapeSelectedTemp[0];
	_isShapeSelected[1] = isShapeSelectedTemp[1];
	_isShapeSelected[2] = isShapeSelectedTemp[2];

	std::vector<bool> isShapeCreatedTemp = j["IsShapeCreated"];
	_isShapeCreated[0] = isShapeCreatedTemp[0];
	_isShapeCreated[1] = isShapeCreatedTemp[1];
	_isShapeCreated[2] = isShapeCreatedTemp[2];

	sphereRadius = j["SphereRadius"];

	std::vector<float> cylRadiusHeightTemp = j["CylinderRadiusHeight"];
	cylRadiusHeight = { cylRadiusHeightTemp[0], cylRadiusHeightTemp[1] };

	if (j["HasPhysBody"])
	{
		temporalMass = j["Mass"];
		CreateCollider();
		_physBody->mass = temporalMass;

		_physBody->isStatic = j["IsStatic"];
		_physBody->isKinematic = j["IsKinematic"];
		_physBody->isTrigger = j["IsTrigger"];

		std::vector<float> newGrav = j["Gravity"];
		SetGravity({ newGrav[0], newGrav[1], newGrav[2] });

		if (j.contains("AngularFactor"))
		{
			std::vector<float> newAngularFactor = j["AngularFactor"];
			SetAngularFactor({ newAngularFactor[0] ,newAngularFactor[1],newAngularFactor[2] });
		}

		_physBody->isRenderingCol = j["IsRenderingCol"];
		if (j.contains("IsRenderingRayCast"))
		{
			_physBody->isRenderingRayCast = j["IsRenderingRayCast"];
		}

		std::vector<float> colPosTemp = j["ColPosition"];
		_physBody->colPos = { colPosTemp[0], colPosTemp[1], colPosTemp[2] };

		std::vector<float> colRotTemp = j["ColRotation"];
		_physBody->colRot = { colRotTemp[0], colRotTemp[1], colRotTemp[2] };

		std::vector<float> colSclTemp = j["ColScale"];
		_physBody->colScl = { colSclTemp[0], colSclTemp[1], colSclTemp[2] };

		wireframeSize = j["WireframeSize"];

		sphereVerSlices = j["SphereVerSlices"];
		sphereHorSlices = j["SphereHorSlices"];
		cylinderVerSlices = j["CylVerSlices"];

		std::vector<float> RenderColColorTemp = j["RenderColColor"];
		renderColColor[0] = RenderColColorTemp[0];
		renderColColor[1] = RenderColColorTemp[1];
		renderColColor[2] = RenderColColorTemp[2];
		renderColColor[3] = RenderColColorTemp[3];

		if (j.contains("RayCastSize"))
		{
			raycastSize = j["RayCastSize"];
		}
		if (j.contains("RayCastColor"))
		{
			std::vector<float> RenderRayColorTemp = j["RayCastColor"];
			raycastColor[0] = RenderRayColorTemp[0];
			raycastColor[1] = RenderRayColorTemp[1];
			raycastColor[2] = RenderRayColorTemp[2];
			raycastColor[3] = RenderRayColorTemp[3];
		}

		CallUpdateAllPram();
	}
}

#ifdef STANDALONE

void PhysicsComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Physics", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("Mass: ", &temporalMass, 0.01, 0, 99999.999))
		{
			if (_physBody)
			{
				_physBody->mass = temporalMass;
				CallUpdateMass();
			}
		}

		if (!_physBody)
		{
			ImGui::Text("This object has no collider attached to it.");
			ImGui::Text("Select one shape and create one.");

			ImGui::Checkbox("Box", &_isShapeSelected[0]);

			ImGui::Checkbox("Sphere", &_isShapeSelected[1]);

			ImGui::Checkbox("Cylinder", &_isShapeSelected[2]);

			CheckShapes();

			if (_shapeSelected != ColliderShape::NONE)
			{
				if (ImGui::Button("Create Collider"))
				{
					CreateCollider();
					_physBody->isRenderingCol = true;
					_physBody->isRenderingRayCast = false;
				}
			}

		}
		else
		{
			const char* colliderType = "";
			switch (_shapeSelected)
			{
			case ColliderShape::BOX:
			{
				colliderType = "Box";
			}
			break;
			case ColliderShape::SPHERE:
			{
				colliderType = "Sphere";
			}
			break;
			case ColliderShape::CYLINDER:
			{
				colliderType = "Cylinder";
			}
			break;
			}
			std::string colName = "Collider Attached: ";
			colName += colliderType;
			ImGui::Text(colName.c_str());

			if (ImGui::Checkbox("Static", &_physBody->isStatic)) {
				if (_physBody)
				{
					if (_physBody->isStatic)
						_physBody->isKinematic = false;

					CallUpdateAllPram();
				}
			}

			if (ImGui::Checkbox("Kinematic", &_physBody->isKinematic))
			{
				if (_physBody)
				{
					if (_physBody->isKinematic)
						_physBody->isStatic = false;

					CallUpdateAllPram();
				}
			}

			if (ImGui::Checkbox("Trigger", &_physBody->isTrigger))
			{
				if (_physBody)
					CallUpdateAllPram();
			}

			if (ImGui::DragFloat3("Gravity", _gravity, 0.01))
			{
				if (_physBody)
					_physBody->SetGravity(float3(_gravity[0], _gravity[1], _gravity[2]));
			}

			if (ImGui::DragFloat3("Angular Factor", _angularFactor))
			{
				if (_physBody)
					SetAngularFactor({ _angularFactor[0], _angularFactor[1], _angularFactor[2] });
			}

			if (ImGui::Checkbox("Render", &_physBody->isRenderingCol))
			{
				if (_physBody->isRenderingCol)
				{
					switch (_shapeSelected)
					{
					case ColliderShape::SPHERE:
					{
						if (sphereVerSlices < 3) {
							sphereVerSlices = 3;
						}
						if (sphereVerSlices > MAX_VERTICAL_SLICES_SPHERE) {
							sphereVerSlices = MAX_VERTICAL_SLICES_SPHERE;
						}

						if (sphereHorSlices < 1) {
							sphereHorSlices = 1;
						}
						if (sphereHorSlices > MAX_HORIZONTAL_SLICES_SPHERE) {
							sphereHorSlices = MAX_HORIZONTAL_SLICES_SPHERE;
						}
						//Application::Instance()->renderer3D->renderManager.CalculateSphereIndices(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
						//Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
					}
					break;
					case ColliderShape::CYLINDER:
					{
						if (cylinderVerSlices < 3)
						{
							cylinderVerSlices = 3;
						}
						if (cylinderVerSlices > MAX_VERTICAL_SLICES_CYLINDER)
						{
							cylinderVerSlices = MAX_VERTICAL_SLICES_CYLINDER;
						}
						//Application::Instance()->renderer3D->renderManager.CalculateCylinderIndices(&cylinderIndicesComp, cylinderVerSlices);
						//Application::Instance()->renderer3D->renderManager.CalculateCylinderBuffer(&cylinderIndicesComp, cylinderVerSlices);
					}
					break;
					}
				}

			}

			if (_physBody->isRenderingCol)
			{
				ImGui::ColorEdit4("Color", renderColColor);

				ImGui::DragFloat("Line Size: ", &wireframeSize, 0.1f, 0.5f, 100.f);

				switch (_shapeSelected)
				{
				case ColliderShape::SPHERE:
				{
					if (ImGui::DragInt("Ver. Slices: ", &sphereVerSlices, 1, 3, MAX_VERTICAL_SLICES_SPHERE))
					{
						//Application::Instance()->renderer3D->renderManager.CalculateSphereIndices(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
						//Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
					}
					if (ImGui::DragInt("Hor. Slices: ", &sphereHorSlices, 1, 1, MAX_HORIZONTAL_SLICES_SPHERE))
					{
						//Application::Instance()->renderer3D->renderManager.CalculateSphereIndices(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
						//Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
					}
				}
				break;
				case ColliderShape::CYLINDER:
				{
					if (ImGui::DragInt("Ver. Slices: ", &cylinderVerSlices, 1, 3, MAX_VERTICAL_SLICES_CYLINDER))
					{
						//Application::Instance()->renderer3D->renderManager.CalculateCylinderIndices(&cylinderIndicesComp, cylinderVerSlices);
						//Application::Instance()->renderer3D->renderManager.CalculateCylinderBuffer(&cylinderIndicesComp, cylinderVerSlices);
					}
				}
				break;
				}
			}

			switch (_shapeSelected)
			{
			case ColliderShape::BOX:
			{
				if (ImGui::DragFloat3("Position: ", _physBody->colPos.ptr(), 0.1))
				{
					CallUpdatePos();
				}

				if (ImGui::DragFloat3("Rotation: ", _physBody->colRot.ptr(), 0.1))
				{
					CallUpdateRotation();
				}

				if (ImGui::DragFloat3("Scale: ", _physBody->colScl.ptr(), 0.1))
				{
					CallUpdateScale();
				}
			}
			break;
			case ColliderShape::SPHERE:
			{
				if (ImGui::DragFloat3("Position: ", _physBody->colPos.ptr(), 0.1))
				{
					CallUpdatePos();
				}

				if (ImGui::DragFloat3("Rotation: ", _physBody->colRot.ptr(), 0.1))
				{
					CallUpdateRotation();
				}
				if (ImGui::DragFloat("Radius: ", &sphereRadius, 0.1))
				{
					CallUpdateScale();
				}
			}
			break;
			case ColliderShape::CYLINDER:
			{
				if (ImGui::DragFloat3("Position: ", _physBody->colPos.ptr(), 0.1))
				{
					CallUpdatePos();
				}
				if (ImGui::DragFloat3("Rotation: ", _physBody->colRot.ptr(), 0.1))
				{
					CallUpdateRotation();
				}
				if (ImGui::DragFloat2("Radius & Height: ", cylRadiusHeight.ptr(), 0.1))
				{
					CallUpdateScale();
				}
			}
			break;
			}

			if (ImGui::Button("Remove Collider"))
			{
				RemoveCollider();
			}

			if (ImGui::CollapsingHeader("RayCast", nullptr))
			{
				ImGui::Checkbox("Render RayCast", &_physBody->isRenderingRayCast);

				if (_physBody->isRenderingRayCast)
				{
					ImGui::ColorEdit4("RayCast Color", raycastColor);

					ImGui::DragFloat("RayCast Size: ", &raycastSize, 0.1f, 0.5f, 100.f);
				}
			}
		}
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}
#endif // STANDALONE

void PhysicsComponent::CheckRenderBuffers()
{
	switch (_shapeSelected)
	{
	case ColliderShape::SPHERE:
	{
		//Application::Instance()->renderer3D->renderManager.CalculateSphereIndices(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
		//Application::Instance()->renderer3D->renderManager.CalculateSphereBuffer(&sphereIndicesComp, sphereVerSlices, sphereHorSlices);
	}
	break;
	case ColliderShape::CYLINDER:
	{
		//Application::Instance()->renderer3D->renderManager.CalculateCylinderIndices(&cylinderIndicesComp, cylinderVerSlices);
		//Application::Instance()->renderer3D->renderManager.CalculateCylinderBuffer(&cylinderIndicesComp, cylinderVerSlices);
	}
	break;
	default:
		break;
	}
}

float PhysicsComponent::GetRadius()
{
	return sphereRadius;
}

void PhysicsComponent::SetAngularFactor(float3 angle)
{
	this->_physBody->body->setAngularFactor({ angle.x, angle.y, angle.z });
}

void PhysicsComponent::CallUpdatePos()
{
	ModulePhysics::UpdatePhysBodyPos(_physBody);
}

void PhysicsComponent::CallUpdateRotation()
{
	ModulePhysics::UpdatePhysBodyRotation(_physBody);
}

void PhysicsComponent::CallUpdateScale()
{
	switch (_shapeSelected)
	{
	case ColliderShape::BOX:
		ModulePhysics::UpdatePhysBodyScaleBox(_physBody);
		break;
	case ColliderShape::SPHERE:
		ModulePhysics::UpdatePhysBodyScaleSphere(_physBody, sphereRadius);
		break;
	case ColliderShape::CYLINDER:
		ModulePhysics::UpdatePhysBodyScaleCylinder(_physBody, cylRadiusHeight.x, cylRadiusHeight.y);
		break;
	}
}

void PhysicsComponent::CallUpdateMass()
{
	if (!_physBody->isStatic && !_physBody->isKinematic)
	{
		btVector3 inertia;
		btCollisionShape* shape = _physBody->body->getCollisionShape();
		shape->calculateLocalInertia(_physBody->mass, inertia);
		_physBody->body->setMassProps(_physBody->mass, inertia);
	}
	else
	{
		btVector3 inertia(0, 0, 0);
		_physBody->body->setMassProps(0, inertia);
	}
}

void PhysicsComponent::CallUpdateColliderType()
{
	if (_physBody->isStatic == true)
	{
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}
	else
	{
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
	}

	if (_physBody->isKinematic == true)
	{
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	else
	{
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	CallUpdateMass();
}

void PhysicsComponent::CallUpdateTrigger()
{
	if (_physBody->isTrigger == true)
	{
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void PhysicsComponent::CallUpdateAllPram()
{
	CallUpdateColliderType();
	CallUpdateTrigger();

	CallUpdatePos();
	CallUpdateRotation();
	CallUpdateScale();
}

void PhysicsComponent::CreateCollider()
{
	switch (_shapeSelected)
	{
	case ColliderShape::BOX:
	{
		PrimCube cube;
		cube.transform = cube.transform.identity;

		cube.size.x = 1;//GetScale().x;
		cube.size.y = 1;
		cube.size.z = 1;
		//cube.transform.Translate(_gameObject->GetComponent<TransformComponent>()->GetGlobalPosition());

		_physBody = Application::Instance()->physic->CreatePhysBody(&cube, 1);
	}
	break;
	case ColliderShape::SPHERE:
	{
		PrimSphere sphere;

		sphere.radius = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y * WORLD_TO_RENDER_RELATION;

		_physBody = Application::Instance()->physic->CreatePhysBody(&sphere, 1);
	}
	break;
	case ColliderShape::CYLINDER:
	{
		PrimCylinder cylinder;

		cylinder.radius = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().x * WORLD_TO_RENDER_RELATION;
		cylinder.height = _gameObject->GetComponent<TransformComponent>()->GetGlobalScale().y * WORLD_TO_RENDER_RELATION;

		_physBody = Application::Instance()->physic->CreatePhysBody(&cylinder, 1);
	}
	break;
	}

	_physBody->gameObjectUID = _gameObject->GetID();
	_physBody->SetPos(_physBody->colPos.x, _physBody->colPos.y, _physBody->colPos.z);
	
	CallUpdatePos();
}

void PhysicsComponent::RemoveCollider()
{
	ModulePhysics::S_RemovePhysBody(_physBody);
	_physBody = nullptr;
}

void PhysicsComponent::OnTransformCallback(float4x4 worldMatrix)
{
	if (_gameObject->transform->_ignorePhysBody) // If this transformation has to be ignored...
	{
		_gameObject->transform->_ignorePhysBody = false; // ... set the flag to false, so next transformation doesn't get ignored, and return.
		return;
	}

	if (_physBody != nullptr)
	{
		CallUpdatePos();
		CallUpdateRotation();
	}
}

void PhysicsComponent::OnEnable()
{
	if (_physBody != nullptr)
	{
		_physBody->body->forceActivationState(ACTIVE_TAG);
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() | (this->_isStatic ? btCollisionObject::CollisionFlags::CF_STATIC_OBJECT : btCollisionObject::CollisionFlags::CF_CHARACTER_OBJECT));
	}
}

void PhysicsComponent::OnDisable()
{
	if (_physBody != nullptr)
	{
		_physBody->body->forceActivationState(DISABLE_SIMULATION);
		_physBody->body->setCollisionFlags(_physBody->body->getCollisionFlags() | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
	}
}

void PhysicsComponent::CheckShapes()
{
	for (int h = 0; h < static_cast<int>(ColliderShape::Count); h++) {

		if (_isShapeSelected[h] == true) {
			if (_isShapeCreated[h] == false)
			{
				_shapeSelected = static_cast<ColliderShape>(h);
				_isShapeCreated[h] = true;
			}
		}
	}

	bool hasCount = false;
	for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++)
	{
		if (_isShapeSelected[i] == true)
		{
			hasCount = true;
			break;
		}
	}

	if (!hasCount)
	{
		_shapeSelected = ColliderShape::NONE;
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++)
		{
			_isShapeCreated[i] = false;
		}
	}

	if (_shapeSelected == ColliderShape::NONE)
	{
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++)
		{
			if (_isShapeSelected[i] == true)
			{
				_isShapeSelected[i] = false;

				if (_isShapeCreated[i] == true)
					_isShapeCreated[i] = false;
			}
		}
	}
	else
	{
		for (int i = 0; i < static_cast<int>(ColliderShape::Count); i++) {

			if (i != static_cast<int>(_shapeSelected))
			{
				if (_isShapeSelected[i] == true)
				{
					_isShapeSelected[i] = false;

					if (_isShapeCreated[i] == true)
						_isShapeCreated[i] = false;
				}
			}
		}
	}
}
