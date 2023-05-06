#include "Headers.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCommand.h"

TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::TRANSFORM;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(float3 pos)
{
	this->localTransform.position = pos;
	UpdateDirtyFlag();
}

void TransformComponent::SetScale(float3 s)
{
	this->localTransform.scale = s;
	UpdateDirtyFlag();
}

void TransformComponent::SetRotation(float3 rot)
{
	this->localTransform.rotation = rot;
	UpdateDirtyFlag();
}

void TransformComponent::SetTransform(float3 pos, float3 scale, float3 rot)
{
	this->localTransform.position = pos;
	this->localTransform.scale = scale;
	this->localTransform.rotation = rot;
	UpdateDirtyFlag();
}

void TransformComponent::SetTransform(float4x4& localTransformMatrix, bool ignoreRotation, bool ignoreChildren, bool ignoreScale)
{
	Quat rotation;
	float3 scale;

	if (!ignoreScale)
		localTransformMatrix.Decompose(localTransform.position, rotation, localTransform.scale);
	else
		localTransformMatrix.Decompose(localTransform.position, rotation, scale);

	if (!ignoreRotation)
		localTransform.rotation = RadToDeg(rotation.ToEulerXYZ());
		
	_localMatrix = localTransformMatrix;

	for (auto& component : _gameObject->_components)
	{
		if (component->NeedsTransformCallback()) // Check if we need to callback our transform to some component.
		{
			component->OnTransformCallback(GetGlobalMatrix(true, false));
		}
	}

	_dirtyFlag = true;
	if (!ignoreChildren)
		UpdateDirtyFlagForChildren();
}

void TransformComponent::SetTransform(float* matrix)
{
	_localMatrix.Set(matrix);

	UpdateDirtyFlagNoLocal();
}

void TransformComponent::SetLocalFromGlobal(float4x4& globalMatrix, bool ignoreRotation, bool ignoreScale)
{
	float4x4 parentGlobal = _gameObject->_parent->transform->_globalMatrix;
	parentGlobal.InverseAccurate();

	float4x4 localTransformed = parentGlobal * globalMatrix;
	SetTransform(localTransformed, ignoreRotation, false, ignoreScale);
}

void TransformComponent::SetLocalFromGlobalIgnoreChildren(float4x4& globalMatrix)
{
	float4x4 parentGlobal = _gameObject->_parent->transform->_globalMatrix;
	parentGlobal.InverseAccurate();

	float4x4 localTransformed = parentGlobal * globalMatrix;
	SetTransform(localTransformed, false, true);
}

void TransformComponent::Translate(float3 translation)
{
	this->localTransform.position += translation;
	UpdateDirtyFlag();
}

void TransformComponent::Scale(float3 s)
{
	this->localTransform.scale += s;
	UpdateDirtyFlag();
}

void TransformComponent::Rotate(float3 rotate)
{
	this->localTransform.rotation += rotate;
	UpdateDirtyFlag();
}

float4x4 TransformComponent::GetGlobalMatrix(bool forceUpdate, bool updateLocal)
{
	if (updateLocal)
		CalculateLocalMatrix();
	if (_dirtyFlag || forceUpdate)
	{
		float4x4 parentGlobal = _gameObject->_parent != nullptr ? _gameObject->_parent->transform->GetGlobalMatrix() : float4x4::identity;
		_globalMatrix = parentGlobal * _localMatrix;
		_dirtyFlag = false;
		forceUpdate = false;
	}
	return _globalMatrix;
}

float3 TransformComponent::GetGlobalPosition()
{
	return GetGlobalMatrix().TranslatePart();
}

float3 TransformComponent::GetGlobalRotation()
{
	float4x4 global = GetGlobalMatrix();
	float3 rotat = global.ToEulerXYZ();
	return rotat;
}

float3 TransformComponent::GetGlobalScale()
{
	return GetGlobalMatrix().GetScale();
}

float3 TransformComponent::GetLocalPosition()
{
	return localTransform.position;
}

float3 TransformComponent::GetLocalRotation()
{
	return localTransform.rotation;
}

float3 TransformComponent::GetLocalScale()
{
	return localTransform.scale;
}

float3 TransformComponent::GetForward()
{
	return GetGlobalMatrix().RotatePart().Col(2).Normalized();
}

float3 TransformComponent::GetRight()
{
	return GetGlobalMatrix().RotatePart().Col(0).Normalized();
}

float3 TransformComponent::GetUp()
{
	return GetGlobalMatrix().RotatePart().Col(1).Normalized();
}

#ifdef STANDALONE
void TransformComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("position", &localTransform.position[0], 0.1f))
		{
			UpdateDirtyFlag();
		}

		if(ImGui::IsItemActivated())
		{
			tempTransform = localTransform;
		}
		if(ImGui::IsItemDeactivated())
		{
			if(tempTransform != localTransform)
			{
				ModuleCommand::S_ChangeValue(&localTransform, tempTransform, localTransform, 
					std::bind(&TransformComponent::UpdateDirtyFlag, this));
			}
		}
		if (ImGui::DragFloat3("rotation", &localTransform.rotation[0], 0.1f))
		{
			UpdateDirtyFlag();
		}
		if (ImGui::IsItemActivated())
		{
			tempTransform = localTransform;
		}
		if (ImGui::IsItemDeactivated())
		{
			if (tempTransform != localTransform)
			{
				ModuleCommand::S_ChangeValue(&localTransform, tempTransform, localTransform, 
					std::bind(&TransformComponent::UpdateDirtyFlag, this));
			}
		}

		if (ImGui::DragFloat3("scale", &localTransform.scale[0], 0.1f))
		{
			UpdateDirtyFlag();
		}
		if (ImGui::IsItemActivated())
		{
			tempTransform = localTransform;
		}
		if (ImGui::IsItemDeactivated())
		{
			if (tempTransform != localTransform)
			{
				ModuleCommand::S_ChangeValue(&localTransform, tempTransform, localTransform, 
					std::bind(&TransformComponent::UpdateDirtyFlag, this));
			}
		}
	}
}
#endif
void TransformComponent::ForceUpdate()
{
	UpdateDirtyFlag();
}

void TransformComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;
	_j["Position"] = { localTransform.position.x, localTransform.position.y, localTransform.position.z };
	_j["Rotation"] = { localTransform.rotation.x, localTransform.rotation.y, localTransform.rotation.z };
	_j["Scale"] = { localTransform.scale.x, localTransform.scale.y, localTransform.scale.z };
	_j["Enabled"] = _isEnabled;
	j["Components"].push_back(_j);
}

void TransformComponent::DeSerialization(json& j)
{
	std::vector<float> tempPos = j["Position"];
	localTransform.position = { tempPos[0], tempPos[1], tempPos[2] };
	std::vector<float> tempRot = j["Rotation"];
	localTransform.rotation = { tempRot[0], tempRot[1], tempRot[2] };
	std::vector<float> tempScale = j["Scale"];
	localTransform.scale = { tempScale[0], tempScale[1], tempScale[2] };

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();

	_dirtyFlag = true;
}

void TransformComponent::UpdateDirtyFlagForChildren()
{
	std::vector<TransformComponent*> childrenTransforms;
	for (int i = 0; i < _gameObject->_children.size(); i++)
	{
		childrenTransforms.push_back((TransformComponent*)_gameObject->_children[i]->_components[0]); // We assume component N?0 is always Transform.
	}

	for (auto& transfrom : childrenTransforms)
	{
		transfrom->UpdateDirtyFlag();
	}
}

void TransformComponent::CalculateLocalMatrix()
{
	Quat rot = Quat::FromEulerXYZ(math::DegToRad(localTransform.rotation.x), math::DegToRad(localTransform.rotation.y), math::DegToRad(localTransform.rotation.z));
	_localMatrix = _localMatrix.FromTRS(localTransform.position, rot, localTransform.scale); // Calculate local matrix from our local transfom values.
}

void TransformComponent::UpdateDirtyFlagNoLocal()
{
	for (auto& component : _gameObject->_components)
	{
		if (component->NeedsTransformCallback()) // Check if we need to callback our transform to some component.
		{
			component->OnTransformCallback(GetGlobalMatrix(true, false));
		}
	}

	_dirtyFlag = true;
	UpdateDirtyFlagForChildren();
}

void TransformComponent::UpdateDirtyFlag()
{
	_dirtyFlag = true;
	for (auto& component : _gameObject->_components)
	{
		if (component->NeedsTransformCallback()) // Check if we need to callback our transform to some component.
		{
			component->OnTransformCallback(GetGlobalMatrix());
		}
	}

	UpdateDirtyFlagForChildren();
}

