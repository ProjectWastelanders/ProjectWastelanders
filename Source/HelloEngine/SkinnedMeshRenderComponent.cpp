#include "Headers.h"
#include "SkinnedMeshRenderComponent.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "LayerEditor.h"
#include "ModuleLayers.h"
#include "TransformComponent.h"
#include "TextureComponent.h"

SkinnedMeshRenderComponent::SkinnedMeshRenderComponent(GameObject* gameObject) : MeshRenderComponent(gameObject)
{
	_type = Type::SKINNING;
	_meshID = -1;
	_instanceID = 0;
	_needsTransformCallback = true;
	_hasBones = false;
}

SkinnedMeshRenderComponent::SkinnedMeshRenderComponent(GameObject* gameObject, const SkinnedMeshRenderComponent& copy) : MeshRenderComponent(gameObject, copy)
{
	
}

SkinnedMeshRenderComponent::~SkinnedMeshRenderComponent()
{
	goBonesArr.clear();
	rootBone = nullptr;
}

void SkinnedMeshRenderComponent::CreateMesh(uint resourceUID, uint materialUID, MeshRenderType type)
{
	MeshRenderComponent::CreateMesh(resourceUID, materialUID, MeshRenderType::INDEPENDENT);

}
#ifdef STANDALONE

void SkinnedMeshRenderComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Skinned Mesh Renderer", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (_meshID == -1)
		{
			ImGui::TextWrapped("No mesh loaded! Drag an .hmesh file below to load a mesh ");

			// Create Droped mesh
			MeshDropArea();
			ImGui::HelpMarker("You can find .hmesh files by clicking on any model file (FBX or DAE). They will appear below the file icon in the Project window.");

			return;
		}

		bool auxiliaryBool = _isEnabled;
		if (ImGui::Checkbox("Active##Mesh", &auxiliaryBool))
			auxiliaryBool ? Enable() : Disable();

		Mesh& mesh = GetMesh();

		ImGui::TextWrapped("Mesh vertices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(_vertexNum).c_str());

		ImGui::TextWrapped("Mesh indices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(_indexNum).c_str());

		ImGui::Checkbox("Vertex Normals", &mesh.showVertexNormals); ImGui::SameLine();
		ImGui::Checkbox("Face Normals", &mesh.showFaceNormals);

		ImGui::Checkbox("AABB", &mesh.showAABB); ImGui::SameLine();
		ImGui::Checkbox("OBB", &mesh.showOBB);

		//Root bone gameObject drop area
		RootBoneDropArea();

		ImGui::TextColored(ImVec4(1, 1, 0, 1), _resource->debugName.c_str()); ImGui::SameLine();
		// Create Droped mesh
		MeshDropArea();

		//Has Bones user feedback
		if (!_hasBones)
		{
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "This mesh doesn't have bones, consider using 'Mesh Renderer' component.");
		}

		ImGui::HelpMarker("You can find .hmesh files by clicking on any model file (FBX or DAE). They will appear below the file icon in the Project window.");
	}
	if (!created)
		this->_gameObject->DestroyComponent(this);
}

#endif
void SkinnedMeshRenderComponent::RootBoneDropArea()
{
	ImGui::NewLine();
	
	std::string dragText = "Drag root bone here";
	if (rootBone != nullptr) dragText = rootBone->name;

	ImGui::Button(dragText.c_str());

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{

			rootBone = LayerEditor::draggedGameObject;
			goBonesArr.clear();
			LinkBones(rootBone, _resource->meshInfo.boneDataMap);
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::NewLine();
}


void SkinnedMeshRenderComponent::UpdateBones(Animation3D* animation, float animationTime)
{
	if (rootBone == nullptr) return;
	goBonesArr.clear();
	LinkBones(rootBone, _resource->meshInfo.boneDataMap, animation, float4x4::identity, animationTime);
}

void SkinnedMeshRenderComponent::SetRootBone(GameObject* nextRootBone)
{
	rootBone = nextRootBone;

	goBonesArr.clear();
	LinkBones(rootBone, _resource->meshInfo.boneDataMap);
}

void SkinnedMeshRenderComponent::LinkBones(GameObject* goBone, std::map<std::string, BoneData>& boneDataMap, Animation3D* animation, float4x4 parentTransform, float animationTime)
{
	if (boneDataMap.count(goBone->name))
	{
		float4x4 offset = boneDataMap.at(goBone->name).offset;
		float4x4 delta = goBone->GetComponent<TransformComponent>()->GetGlobalMatrix();

		if (animation != nullptr)
		{
			AnimatedBone* animBone = nullptr;

			if (animBone = animation->FindBone(goBone->name))
			{
				parentTransform = parentTransform * animBone->GetTransform(animationTime);
				if (goBone->GetBonesUpdate())
				{
					float4x4 positionInGame = _gameObject->transform->GetGlobalMatrix() * parentTransform;
					goBone->transform->SetLocalFromGlobal(positionInGame, false, true);
				}
			}
			else
			{
				parentTransform = parentTransform * delta;
			}

			delta = parentTransform * offset;

			
		}
		else
		{
			delta = _gameObject->transform->GetGlobalMatrix().Inverted() * delta;

			delta = delta * offset;
		}

		goBonesArr.push_back(delta);
	}
	for (int i = 0; i < goBone->GetChildren()->size(); ++i)
	{
		LinkBones(goBone->GetChildren()->at(i), boneDataMap, animation, parentTransform, animationTime);
	}
}

void SkinnedMeshRenderComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

	if (_meshID != -1)
	{
		Mesh& m = GetMesh();
		_j["ShowAABB"] = m.showAABB;
		_j["ShowOBB"] = m.showOBB;
		_j["ShowFaceNormals"] = m.showFaceNormals;
		_j["ShowVertexNormals"] = m.showVertexNormals;
	}
	else
	{
		_j["ShowAABB"] = false;
		_j["ShowOBB"] = false;
		_j["ShowFaceNormals"] = false;
		_j["ShowVertexNormals"] = false;
	}

	if (_resource != nullptr)
	{
		_j["ModelUID"] = _resource->modelUID;
		_j["Index inside model"] = _resource->indexInsideModel;
	}
	else
	{
		_j["ModelUID"] = 0;
		_j["Index inside model"] = 0;
	}

	if (rootBone != nullptr)
	{
		_j["RootBoneUuid"] = rootBone->GetID();
	}
	else
	{
		_j["RootBoneUuid"] = -1;
	}
	_j["Enabled"] = _isEnabled;


	j["Components"].push_back(_j);
}

void SkinnedMeshRenderComponent::DeSerialization(json& j)
{
	ResourceModel* model = (ResourceModel*)ModuleResourceManager::S_LoadResource(j["ModelUID"]);

	if (model == nullptr)
	{
		Console::S_Log("A scene skinned mesh render data was not found.");
		return;
	}

	uint index = j["Index inside model"];
	if (index < model->modelMeshes.size())
	{
		ResourceMesh* resourceMesh = model->modelMeshes[index];

		CreateMesh(resourceMesh->UID);

		if (_meshID != -1)
		{
			Mesh& m = GetMesh();

			m.showAABB = j["ShowAABB"];
			m.showOBB = j["ShowOBB"];
			m.showFaceNormals = j["ShowFaceNormals"];
			m.showVertexNormals = j["ShowVertexNormals"];
		}
	}

	//Bones
	uint rootBoneUuid = j["RootBoneUuid"];
	if (rootBoneUuid > 0)
	{
		if (ModuleLayers::gameObjects.count(rootBoneUuid) != 0)
			rootBone = ModuleLayers::gameObjects[rootBoneUuid];
		else
			rootBone = nullptr;
		UpdateBones();
	}

	bool enabled = j["Enabled"];
	if (!enabled)
		Disable();
}