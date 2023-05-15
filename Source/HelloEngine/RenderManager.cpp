#include "Headers.h"
#include "ModulePhysics.h"
#include "RenderManager.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "MeshImporter.h"
#include "CameraObject.h"
#include "ModuleCamera3D.h"
#include "ComponentUIButton.h"
#include "ComponentUISlider.h"
#include "ComponentUICheckbox.h"
#include "ComponentUIImage.h"
#include "TextRendererComponent.h"
#include "Math/float4x4.h"
#include "LayerEditor.h"
#include "PhysicsComponent.h"
#include "VideoPlayerComponent.h"

#include "ModuleRenderer3D.h"

#include "ModuleNavMesh.h"
#include "NavMeshBuilder.h"

#include "LayerEditor.h"
#include "NavAgentComponent.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
	RELEASE(_textureManager);

	if (lineShader)
	{
		lineShader->Dereference();
		lineShader = nullptr;
	}
	if (localLineShader)
	{
		localLineShader->Dereference();
		localLineShader = nullptr;
	}
	RELEASE(textRenderingShader);

}

void RenderManager::Init()
{
	_textureManager = new TextureManager();
	FontManager::InitFreetype();

	cubeUID = 2523359550;
	planeUID = 90291865;
	cylinderUID = 2734209052;
	sphereUID = 4243659739;
	plane2DUID = 90291866;

	// Create resources for Primitives
	ResourceMesh* cubeResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2523359550.hmesh", cubeUID, "Cube", false);
	ResourceMesh* planeResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/90291865.hmesh", planeUID, "Plane", false);
	ResourceMesh* plane2DResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/90291865.hmesh", plane2DUID, "Plane2D", false);
	ResourceMesh* cylinderResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/2734209052.hmesh", cylinderUID, "Cylinder", false);
	ResourceMesh* sphereResource = ModuleResourceManager::S_CreateResourceMesh("Resources/Editor/Primitives/4243659739.hmesh", sphereUID, "Sphere", false);

	for (int i = 0; i < 5; ++i)
	{
		primitiveModels[i] = new ResourceModel();
	}

	// Create a Resource Model for every primitive. This is mandatory so Serialization can funciton properly.

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::CUBE] = primitiveModels[(int)PrimitiveType::CUBE];
	cubeResource->modelUID = (uint)PrimitiveModelsUID::CUBE;
	primitiveModels[(int)PrimitiveType::CUBE]->modelMeshes.push_back(cubeResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::CYLINDER] = primitiveModels[(int)PrimitiveType::CYLINDER];
	cylinderResource->modelUID = (uint)PrimitiveModelsUID::CYLINDER;
	primitiveModels[(int)PrimitiveType::CYLINDER]->modelMeshes.push_back(cylinderResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::PLANE] = primitiveModels[(int)PrimitiveType::PLANE];
	planeResource->modelUID = (uint)PrimitiveModelsUID::PLANE;
	primitiveModels[(int)PrimitiveType::PLANE]->modelMeshes.push_back(planeResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::PLANE2D] = primitiveModels[(int)PrimitiveType::PLANE2D];
	plane2DResource->modelUID = (uint)PrimitiveModelsUID::PLANE2D;
	primitiveModels[(int)PrimitiveType::PLANE2D]->modelMeshes.push_back(plane2DResource);

	ModuleResourceManager::resources[(uint)PrimitiveModelsUID::SPHERE] = primitiveModels[(int)PrimitiveType::SPHERE];
	sphereResource->modelUID = (uint)PrimitiveModelsUID::SPHERE;
	primitiveModels[(int)PrimitiveType::SPHERE]->modelMeshes.push_back(sphereResource);

	// Init shaders
	lineShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/lines.shader", 100, "Lines");
	localLineShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/localLines.shader", 101, "Local Lines");
	textRenderingShader = new Shader("Resources/shaders/textRendering.shader");
	instancedShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/instanced.shader", 102, "Instanced");

	// Set up debug drawing variables:
	// Manually created box index buffer that corresponds to the order given by MathGeoLib's AABB class GetCornerPoints() method.
	boxIndices.push_back(0);    // 1
	boxIndices.push_back(1);    // 2
	boxIndices.push_back(0);    // 3
	boxIndices.push_back(2);    // 4
	boxIndices.push_back(2);    // 5
	boxIndices.push_back(3);    // 6
	boxIndices.push_back(1);    // 7
	boxIndices.push_back(3);    // 8
	boxIndices.push_back(0);    // 9
	boxIndices.push_back(4);    // 10
	boxIndices.push_back(4);    // 11
	boxIndices.push_back(5);    // 12
	boxIndices.push_back(4);    // 13
	boxIndices.push_back(6);    // 14
	boxIndices.push_back(6);    // 15
	boxIndices.push_back(7);    // 16
	boxIndices.push_back(7);    // 17
	boxIndices.push_back(5);    // 18
	boxIndices.push_back(1);    // 19
	boxIndices.push_back(5);    // 20
	boxIndices.push_back(3);    // 21
	boxIndices.push_back(7);    // 22
	boxIndices.push_back(2);    // 23
	boxIndices.push_back(6);    // 24

	 // Set up buffer for OBB lines.
	glGenVertexArrays(1, &OBBVAO);
	glBindVertexArray(OBBVAO);

	glGenBuffers(1, &OBBIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OBBIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &OBBVBO);
	glBindBuffer(GL_ARRAY_BUFFER, OBBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

	// Set up buffer for AABB lines.
	glGenVertexArrays(1, &AABBVAO);
	glBindVertexArray(AABBVAO);

	glGenBuffers(1, &AABBIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, AABBIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(), &boxIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &AABBVBO);
	glBindBuffer(GL_ARRAY_BUFFER, AABBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 8, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

	// text rendering
	glGenVertexArrays(1, &TextVAO);
	glGenBuffers(1, &TextVBO);
	glBindVertexArray(TextVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CalculateSphereIndices(&sphereIndicesMax, MAX_VERTICAL_SLICES_SPHERE, MAX_HORIZONTAL_SLICES_SPHERE);
	CalculateSphereBuffer(&sphereIndicesMax, MAX_VERTICAL_SLICES_SPHERE, MAX_HORIZONTAL_SLICES_SPHERE);

	CalculateCylinderIndices(&cylinderIndicesMax, MAX_VERTICAL_SLICES_CYLINDER);
	CalculateCylinderBuffer(&cylinderIndicesMax, MAX_VERTICAL_SLICES_CYLINDER);

	// RayCast Line
	rayCastLineIndices.push_back(0);    // 1
	rayCastLineIndices.push_back(1);    // 2

	 // Set up buffer for Raycast line.
	glGenVertexArrays(1, &RAYVAO);
	glBindVertexArray(RAYVAO);

	glGenBuffers(1, &RAYIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RAYIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* rayCastLineIndices.size(), &rayCastLineIndices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &RAYVBO);
	glBindBuffer(GL_ARRAY_BUFFER, RAYVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 2, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);

}

void RenderManager::OnEditor()
{
	ImVec4 textColour = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);

	if (ImGui::Begin("Render System Debugging"))
	{
		ImGui::TextWrapped("Window to manage every RenderManager memory usage and instances. The memory allocated per RenderManager increases by 50 per cent every time the current allocated memory gets surpassed.");
		for (auto& manager : _renderMap)
		{
			if (manager.second.resource == nullptr)
				continue;
			std::string headerName = manager.second.resource->debugName + "##" + std::to_string(manager.second.GetRenderID());
			if (ImGui::CollapsingHeader(headerName.c_str()))
			{
				std::string maxInstances = "Maximum number of instances: " + std::to_string(manager.second.instanceNum);
				ImGui::Text(maxInstances.c_str());

				ImGui::Text("Instances: "); ImGui::SameLine();
				ImGui::TextColored(textColour, std::to_string(manager.second.meshes.size()).c_str());

				ImGui::Text("Memory usage in bytes: "); ImGui::SameLine();
				float memory = manager.second.instanceNum * sizeof(float4x4) + manager.second.instanceNum * sizeof(float);
				memory += manager.second.totalIndices->size() * sizeof(uint);
				memory += manager.second.totalVertices->size() * sizeof(Vertex);
				ImGui::TextColored(textColour, std::to_string(memory).c_str());

				if (manager.second.resMat)
				{
					std::string headerMaterial = manager.second.resMat->debugName + "##" + std::to_string(manager.second.resMat->UID);
					if (ImGui::CollapsingHeader(headerMaterial.c_str()))
					{
						ImGui::Text("Shader: "); ImGui::SameLine();
						if (manager.second.resMat->material.GetShader())
							ImGui::TextColored(textColour, manager.second.resMat->material.GetShader()->debugName.c_str());
						else
							ImGui::TextColored(textColour, "Nullptr");

						ImGui::Text("Number of uniforms: "); ImGui::SameLine();
						ImGui::TextColored(textColour, std::to_string(manager.second.resMat->material.uniforms.size()).c_str());
					}
				}
				else
				{
					ImGui::TextColored(textColour, "Not using a Material");
				}
			}
		}
	}
	ImGui::End();
}

InstanceRenderer* RenderManager::GetRenderManager(uint meshID, uint materialID, bool create)
{
	uint ID = meshID + materialID;
	ResourceMaterial* mat = nullptr;

	//It has to check first or otherwise a NULL resource could be generated
	if (materialID > 0)
		if (ModuleResourceManager::resources.count(materialID))
			mat = (ResourceMaterial*)ModuleResourceManager::resources[materialID];

	// If there is no instance Renderer for this mesh resource
	if (_renderMap.count(ID) == 0)
	{
		if (create)
		{
			_renderMap[ID].SetMeshInformation((ResourceMesh*)ModuleResourceManager::resources[meshID],
				mat);
		}
		else
			return nullptr;
	}

	return &_renderMap[ID];
}

void RenderManager::Draw()
{
	OPTICK_EVENT();
	instancedShader->shader.data.hasUpdatedCamera = false;

	// Draw opaque meshes instanced.
	for (auto& obj : _renderMap)
	{
		obj.second.Draw();
	}
	// Delete empty render managers.
	for (int i = 0; i < _emptyRenderManagers.size(); i++)
	{
		_renderMap.erase(_emptyRenderManagers[i]);
	}
	_emptyRenderManagers.clear();

	// Draw meshes that must be rendered in an individual draw call.
	DrawIndependentMeshes();
	// Draw selected mesh
	DrawSelectedMesh();

	// Draw meshes that have transparency textures applied on their material.
	DrawTransparentMeshes();
}

void RenderManager::DrawDebug()
{
	if (Application::Instance()->renderer3D->isRenderingColliders)
	{
		for (int i = 0; i < ModulePhysics::physBodies.size(); i++)
		{
			ModulePhysics::physBodies[i]->RenderCollider();
			ModulePhysics::physBodies[i]->RenderRayCast();
		}
	}
	else
	{
		if (LayerEditor::selectedGameObject != nullptr)
		{
			PhysicsComponent* physComp = LayerEditor::selectedGameObject->GetComponent<PhysicsComponent>();
			if (physComp)
			{
				for (int i = 0; i < ModulePhysics::physBodies.size(); i++)
				{
					if (ModuleLayers::gameObjects.count(ModulePhysics::physBodies[i]->gameObjectUID) != 0)
					{
						GameObject* go = ModuleLayers::gameObjects[ModulePhysics::physBodies[i]->gameObjectUID];
						if (go == LayerEditor::selectedGameObject)
						{
							ModulePhysics::physBodies[i]->RenderCollider();
							ModulePhysics::physBodies[i]->RenderRayCast();
						}
					}
				}
			}

		}
	}
	
	if (ModuleRenderer3D::drawNavMesh && ModuleNavMesh::S_GetNavMeshBuilder())
	{
		ModuleNavMesh::S_GetNavMeshBuilder()->DebugDraw();

		if (LayerEditor::selectedGameObject && 
			LayerEditor::selectedGameObject->GetComponent<ComponentAgent>())
		{
			ModuleNavMesh::S_GetPathfinding()->RenderPath(LayerEditor::selectedGameObject->GetComponent<ComponentAgent>());
		}		
	}
}

void RenderManager::Draw2D()
{
	// Draw all 2D meshes.
	if (renderer2D != nullptr)
		renderer2D->Draw2D();

	DrawTextObjects();
}

uint RenderManager::AddMesh(ResourceMesh* resource, uint resMat, MeshRenderType type)
{
	switch (type)
	{
	case MeshRenderType::INSTANCED:
		return AddInstancedMesh(resource, resMat);
	case MeshRenderType::INDEPENDENT:
		return AddIndependentMesh(resource, resMat);
	case MeshRenderType::TRANSPARENCY:
		return AddTransparentMesh(resource, resMat);
	case MeshRenderType::MESH2D:
		return Add2DMesh();
	}
}

uint RenderManager::AddTransparentMesh(ResourceMesh* resource, uint resMat)
{
	uint randomID = HelloUUID::GenerateUUID();

	_transparencyMeshes[randomID].mesh.InitWithResource(resource);
	_transparencyMeshes[randomID].mesh.localAABB = resource->localAABB;
	_transparencyMeshes[randomID].mesh.isIndependent = true;
	_transparencyMeshes[randomID].mesh.CreateBufferData();

	//Set Material
	if (resMat > 0)
	{
		ResourceMaterial* mat = (ResourceMaterial*)ModuleResourceManager::S_LoadResource(resMat);
		_transparencyMeshes[randomID].resMat = resMat;
		_transparencyMeshes[randomID].material = mat;

		mat->Dereference();
	}
	/*else
	{
		_independentMeshes[randomID].material = Material(defaultShader.drawPerMesh->UID);
	}*/
	return randomID;
}

uint RenderManager::AddIndependentMesh(ResourceMesh* resource, uint resMat)
{
	uint randomID = HelloUUID::GenerateUUID();

	//Set Mesh
	_independentMeshes[randomID].mesh.InitWithResource(resource);
	_independentMeshes[randomID].mesh.localAABB = resource->localAABB;
	_independentMeshes[randomID].mesh.isIndependent = true;
	_independentMeshes[randomID].mesh.CreateBufferData();

	//Set Material
	if (resMat > 0) //Has been given a proper material UID
	{
		ResourceMaterial* mat = (ResourceMaterial*)ModuleResourceManager::S_LoadResource(resMat);
		_independentMeshes[randomID].resMat = resMat;
		_independentMeshes[randomID].material = mat;

		mat->Dereference();
	}
	//else // Hasn't been given a proper material UID
	//{
	//	if (resource->meshInfo.boneDataMap.size() > 0) //Has bones
	//	{
	//		//Give a default bone mat
	//		_independentMeshes[randomID].material = Material(defaultShader.boneMesh->UID);
	//	}
	//	else
	//	{
	//		//Give a basic default mat
	//		_independentMeshes[randomID].material = Material(defaultShader.drawPerMesh->UID);
	//	}
	//}

	return randomID;
}

uint RenderManager::AddInstancedMesh(ResourceMesh* resource, uint resMat)
{
	InstanceRenderer* manager = GetRenderManager(resource->UID, resMat); // Create a renderManager.
	return manager->AddMesh();
}

uint RenderManager::Add2DMesh()
{
	InstanceRenderer* manager = GetRenderManager(plane2DUID, 0); // Create a renderManager.
	renderer2D = manager;
	renderer2D->SetAs2D();
	uint ret = manager->AddMesh();
	ResourceMesh* plane2D = (ResourceMesh*)ModuleResourceManager::S_LoadResource(plane2DUID);
	renderer2D->meshes[ret].mesh.InitWithResource(plane2D);
	renderer2D->meshes[ret].mesh.localAABB = plane2D->localAABB;
	renderer2D->meshes[ret].mesh.isIndependent = true;
	renderer2D->meshes[ret].mesh.is2D = true;
	renderer2D->meshes[ret].mesh.CreateBufferData();

	//Set Material
	//renderer2D->meshes[ret].material = Material(defaultShader.drawPerMesh2D->UID);

	return ret;
}

uint RenderManager::AddTextObject(std::string text, float4 color, float2 position, float scale)
{
	uint uid = HelloUUID::GenerateUUID();
	TextObject textObject;
	textObject.text = text;
	textObject.color = color;
	textObject.position = position;
	textObject.scale = scale;
	textObjects[uid] = textObject;
	return uid;
}

void RenderManager::CreatePrimitive(GameObject* parent, PrimitiveType type)
{
	if (parent == nullptr)
		parent = ModuleLayers::rootGameObject;

	switch (type)
	{
	case PrimitiveType::CUBE:
	{
		GameObject* cube = new GameObject(parent, "Cube", "Primitive");
		MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
		meshRenderer->CreateMesh(cubeUID);
		break;
	}
	case PrimitiveType::SPHERE:
	{
		GameObject* cube = new GameObject(parent, "Sphere", "Primitive");
		MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
		meshRenderer->CreateMesh(sphereUID);
		break;
	}
	case PrimitiveType::CYLINDER:
	{
		GameObject* cube = new GameObject(parent, "Cylinder", "Primitive");
		MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
		meshRenderer->CreateMesh(cylinderUID);
		break;
	}
	case PrimitiveType::PLANE:
	{
		GameObject* cube = new GameObject(parent, "Plane", "Primitive");
		MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
		meshRenderer->CreateMesh(planeUID);
		break;
	}
	case PrimitiveType::PLANE2D:
	{
		GameObject* canvas = new GameObject(parent, "Canvas", "UI");

		//GameObject* cube = new GameObject(parent, "Canvas", "Primitive");
		//MeshRenderComponent* meshRenderer = cube->AddComponent<MeshRenderComponent>();
		//meshRenderer->CreateMesh(plane2DUID);
		break;
	}
	}
}

void RenderManager::CreateUI(GameObject* parent, UIType type)
{
	if (parent == nullptr)
		parent = ModuleLayers::rootGameObject;

	switch (type)
	{
		case UIType::BUTTON:
		{
			GameObject* button = new GameObject(parent, "Button", "UI");
			button->AddComponent<ComponentUIButton>();
			button->transform->SetScale({ 0.5f,0.5f,0.0f });
			break;
		}
		case UIType::SLIDER:
		{
			GameObject* slider = new GameObject(parent, "Slider", "UI");
			GameObject* sliderButton = new GameObject(slider, "SliderButton", "UIsliderButton");
			GameObject* sliderBar = new GameObject(slider, "SliderBar", "UIsliderBar");
			//slider->AddComponent<ComponentUISlider>();
			sliderBar->AddComponent<ComponentUISlider>();
			sliderBar->transform->SetScale({ 0.7f,0.1f,0.0f });
			sliderButton->AddComponent<ComponentUISlider>();
			sliderButton->transform->SetScale({ 0.2f,0.2f,0.0f });
			sliderButton->transform->SetPosition({ 0.0f, 0.0f, -0.003f });
			break;
		}
		case UIType::CHECKBOX:
		{
			GameObject* checkBox = new GameObject(parent, "CheckBox", "UI");
			checkBox->AddComponent<ComponentUICheckbox>();
			checkBox->transform->SetScale({ 0.5f,0.5f,0.5f });
			break;
		}
		case UIType::IMAGE:
		{
			GameObject* image = new GameObject(parent, "Image", "UI");
			image->AddComponent<ComponentUIImage>();
			image->transform->SetScale({ 0.5f,0.5f,0.5f });
			break;
		}
		case UIType::TEXT:
		{
			GameObject* image = new GameObject(parent, "Text", "UI");
			image->AddComponent<TextRendererComponent>();
			image->transform->SetScale({ 0.5f,0.5f,0.5f });
			break;
		}
		case UIType::VIDEO	:
		{
			GameObject* video = new GameObject(parent, "Text", "UI");
			video->AddComponent<VideoPlayerComponent>();
			video->transform->SetScale({ 0.5f,0.5f,0.5f });
			break;
		}
	}
}

void RenderManager::DestroyRenderManager(uint managerUID)
{
	_emptyRenderManagers.push_back(managerUID);
	// If we are destroying the 2D renderer, set its pointer to nullptr.
	if (_renderMap[managerUID].is2D)
		renderer2D = nullptr;
}

void RenderManager::SetSelectedMesh(RenderEntry* mesh)
{
	_selectedMeshRaw = nullptr;

	if (_selectedMesh == mesh) return;
	_selectedMesh = mesh;
}

void RenderManager::SetSelectedMesh(Mesh* mesh)
{
	_selectedMesh = nullptr;

	if (_selectedMeshRaw == mesh) return;
	_selectedMeshRaw = mesh;
}

void RenderManager::DrawSelectedMesh()
{
	if (_selectedMesh == nullptr && _selectedMeshRaw == nullptr)
		return;

	if (_selectedMesh)
	{
		if (!_selectedMesh->mesh.component->GetGameObject()->isSelected)
		{
			_selectedMesh = nullptr;
			return;
		}
		_selectedMesh->mesh.DrawAsSelected(_selectedMesh->material->material, _selectedMesh->resMat);
	}
	else if (_selectedMeshRaw != nullptr)
	{
		if (!_selectedMeshRaw->component->GetGameObject()->isSelected)
		{
			_selectedMeshRaw = nullptr;
			return;
		}
		_selectedMeshRaw->DrawAsSelected();
	}
}

void RenderManager::RemoveSelectedMesh()
{
	_selectedMesh = nullptr;
	_selectedMeshRaw = nullptr;
}

void RenderManager::DrawVertexNormals(Mesh* mesh)
{
	lineShader->shader.Bind();
	lineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	lineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	lineShader->shader.SetFloat4("lineColor", 0.36f, 0.75f, 0.72f, 1.0f);

	lineShader->shader.SetMatFloat4v("model", &mesh->modelMatrix.v[0][0]);

	glBindVertexArray(mesh->resource->VertexNormalsVAO);

	glDrawArrays(GL_LINES, 0, mesh->resource->vertexNormals.size());

	glBindVertexArray(0);
}

void RenderManager::DrawFaceNormals(Mesh* mesh)
{
	lineShader->shader.Bind();
	lineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	lineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	lineShader->shader.SetFloat4("lineColor", 0.75f, 0.36f, 0.32f, 1.0f);

	lineShader->shader.SetMatFloat4v("model", &mesh->modelMatrix.v[0][0]);

	glBindVertexArray(mesh->resource->FaceNormalsVAO);

	glDrawArrays(GL_LINES, 0, mesh->resource->faceNormals.size());

	glBindVertexArray(0);
}

void RenderManager::DrawOBB(Mesh* mesh)
{
	float3 OBBPoints[8];

	mesh->globalOBB.GetCornerPoints(OBBPoints);

	glBindVertexArray(OBBVAO);

	glBindBuffer(GL_ARRAY_BUFFER, OBBVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &OBBPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->shader.SetFloat4("lineColor", 1.0f, 0.0f, 0.0f, 1.0f);

	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RenderManager::DrawAABB(Mesh* mesh)
{
	float3 AABBPoints[8];

	mesh->globalAABB.GetCornerPoints(AABBPoints);

	glBindVertexArray(AABBVAO);

	glBindBuffer(GL_ARRAY_BUFFER, AABBVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &AABBPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->shader.SetFloat4("lineColor", 0.0f, 1.0f, 0.0f, 1.0f);

	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RenderManager::DrawColliderBox(PhysBody3D* physBody, float4 color, float wireSize)
{
	float3 AABBPoints[8];

	float mult = WORLD_TO_RENDER_RELATION;

	btBoxShape* shape = (btBoxShape*)physBody->body->getCollisionShape();
	btTransform transform = physBody->body->getWorldTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 origin = transform.getOrigin();

	btVector3 rotationAxis = rotation.getAxis();
	btScalar rotationAngle = rotation.getAngle();

	btVector3 eulerRotation;
	btMatrix3x3 rotationMatrix(rotation);
	rotationMatrix.getEulerZYX(eulerRotation[0], eulerRotation[1], eulerRotation[2]);

	btVector3 aabbMin, aabbMax;
	shape->getAabb(transform, aabbMin, aabbMax);

	float minX = aabbMin.getX();
	float minY = aabbMin.getY();
	float minZ = aabbMin.getZ();
	float maxX = aabbMax.getX();
	float maxY = aabbMax.getY();
	float maxZ = aabbMax.getZ();

	btVector3 corners[8];

	corners[0] = btVector3(minX, minY, minZ);
	corners[1] = btVector3(minX, minY, maxZ);
	corners[2] = btVector3(minX, maxY, minZ);
	corners[3] = btVector3(minX, maxY, maxZ);
	corners[4] = btVector3(maxX, minY, minZ);
	corners[5] = btVector3(maxX, minY, maxZ);
	corners[6] = btVector3(maxX, maxY, minZ);
	corners[7] = btVector3(maxX, maxY, maxZ);

	for (int i = 0; i < 8; i++) {
		shape->getVertex(i, corners[i]);
		corners[i] = transform * corners[i];
	}

	for (int i = 0; i < 8; i++) {
		AABBPoints[i] = { corners[i][0], corners[i][1],corners[i][2] };
	}

	glBindVertexArray(AABBVAO);

	glBindBuffer(GL_ARRAY_BUFFER, AABBVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &AABBPoints[0], 8 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

	localLineShader->shader.SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, boxIndices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);

	glBindVertexArray(0);

}

void RenderManager::DrawColliderSphere(std::vector<float3>* spherePointsComp, std::vector<uint>* sphereIndicesComp, float4 color, float wireSize)
{

	//glGenVertexArrays(1, &SPVAO);
	glBindVertexArray(SPVAO);

	//glGenBuffers(1, &SPIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SPIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sphereIndicesComp->size(), &sphereIndicesComp->at(0), GL_DYNAMIC_DRAW);

	//glGenBuffers(1, &SPVBO);
	glBindBuffer(GL_ARRAY_BUFFER, SPVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * spherePointsComp->size(), nullptr, GL_DYNAMIC_DRAW);

	//glBindVertexArray(0);

	///

	glBindVertexArray(SPVAO);

	glBindBuffer(GL_ARRAY_BUFFER, SPVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &spherePointsComp->at(0), spherePointsComp->size() * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->shader.SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, sphereIndicesComp->size(), GL_UNSIGNED_INT, 0);
	//Console::S_Log(std::to_string(sphereIndicesComp->size()));
	glLineWidth(1.0f);

	glBindVertexArray(0);

}

void RenderManager::DrawColliderCylinder(std::vector<float3>* cylinderPointsComp, std::vector<uint>* cylinderIndicesComp, float4 color, float wireSize)
{

	//glGenVertexArrays(1, &CYVAO);
	glBindVertexArray(CYVAO);

	//glGenBuffers(1, &CYIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CYIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * cylinderIndicesComp->size(), &cylinderIndicesComp->at(0), GL_DYNAMIC_DRAW);

	//glGenBuffers(1, &CYVBO);
	glBindBuffer(GL_ARRAY_BUFFER, CYVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * cylinderPointsComp->size(), nullptr, GL_DYNAMIC_DRAW);

	//glBindVertexArray(0);

	///

	glBindVertexArray(CYVAO);

	glBindBuffer(GL_ARRAY_BUFFER, CYVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &cylinderPointsComp->at(0), cylinderPointsComp->size() * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	localLineShader->shader.SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, cylinderIndicesComp->size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);

	glBindVertexArray(0);
}

void RenderManager::CalculateSphereBuffer(std::vector<uint>* sphereIndicesComp, uint verSlices, uint horSlices)
{

	glGenVertexArrays(1, &SPVAO);
	glBindVertexArray(SPVAO);

	glGenBuffers(1, &SPIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SPIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sphereIndicesComp->size(), &sphereIndicesComp->at(0), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &SPVBO);
	glBindBuffer(GL_ARRAY_BUFFER, SPVBO);
	const uint sphereVertexNum = verSlices * horSlices + 2;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * sphereVertexNum, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void RenderManager::CalculateCylinderBuffer(std::vector<uint>* cylinderIndicesComp, uint verSlices)
{
	// Set up buffer for Cylinder lines.
	glGenVertexArrays(1, &CYVAO);
	glBindVertexArray(CYVAO);

	glGenBuffers(1, &CYIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CYIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * cylinderIndicesComp->size(), &cylinderIndicesComp->at(0), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &CYVBO);
	glBindBuffer(GL_ARRAY_BUFFER, CYVBO);
	const uint cylinderVertexNum = verSlices * 2;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * cylinderVertexNum, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void RenderManager::CalculateSphereIndices(std::vector<uint>* sphereIndicesComp, uint verSlices, uint horSlices)
{
	// Set up buffer for Sphere lines.
	sphereIndicesComp->clear();

	for (int j = 1; j < verSlices + 1; j++) {
		sphereIndicesComp->push_back(0);
		sphereIndicesComp->push_back(j);
	}

	for (int i = 0; i < horSlices - 1; i++) {

		//Vertical
		for (int j = 0; j < verSlices; j++) {
			sphereIndicesComp->push_back((i * verSlices + 1) + j);
			sphereIndicesComp->push_back(((i + 1) * verSlices + 1) + j);
		}

		//Horizontal
		for (int j = 0; j < verSlices; j++) {
			if (j == verSlices - 1) {
				sphereIndicesComp->push_back((i * verSlices + 1) + j);
				sphereIndicesComp->push_back((i * verSlices + 1));
			}
			else {
				sphereIndicesComp->push_back((i * verSlices + 1) + j);
				sphereIndicesComp->push_back((i * verSlices + 1) + j + 1);
			}
		}
	}

	for (int i = 0; i < verSlices; i++) {

		//Vertical
		sphereIndicesComp->push_back(((horSlices - 1) * verSlices + 1) + i);
		sphereIndicesComp->push_back(horSlices * verSlices + 1);

		//Horizontal
		if (i == verSlices - 1) {
			sphereIndicesComp->push_back(((horSlices - 1) * verSlices + 1) + i);
			sphereIndicesComp->push_back((horSlices - 1) * verSlices + 1);
		}
		else {
			sphereIndicesComp->push_back(((horSlices - 1) * verSlices + 1) + i);
			sphereIndicesComp->push_back(((horSlices - 1) * verSlices + 1) + i + 1);
		}
	}
}

void RenderManager::CalculateSpherePoints(PhysBody3D* physBody, std::vector<float3>* spherePointsComp, float radius, uint verSlices, uint horSlices)
{
	const float newRadius = radius / 2.f;

	const float3 origin = (float3)physBody->body->getCenterOfMassTransform().getOrigin();
	const float3 startingPointY = -float3(0, newRadius, 0);
	const float diferenceBetweenSlicesY = (newRadius * 2) / (horSlices + 1);

	const btQuaternion rotBtQuat = physBody->body->getCenterOfMassTransform().getRotation();

	float3 rotAxis = (float3)rotBtQuat.getAxis();
	float rotAngle = (float)rotBtQuat.getAngle();
	const Quat rotQuat = Quat::RotateAxisAngle(rotAxis, rotAngle);

	//std::vector<float3> SpherePoints;
	spherePointsComp->clear();

	float3 tempPointFirst = float3(startingPointY);

	float3 rotatedPointFirst = rotQuat * tempPointFirst;

	rotatedPointFirst += origin;

	spherePointsComp->push_back(rotatedPointFirst);

	for (int i = 1; i < horSlices + 1; i++)
	{
		for (int j = 0; j < verSlices; j++)
		{
			/*float tempY = startingPointY.y + diferenceBetweenSlicesY * i;
			float tempYRad = sqrt(Pow(newRadius, 2) - Pow(-newRadius + diferenceBetweenSlicesY * i, 2));
			float tempX = tempYRad * cos(2 * math::pi * j / verSlices);
			float tempZ = tempYRad * sin(2 * math::pi * j / verSlices);

			float3 tempPoint = float3(tempX, tempY, tempZ);

			float3 rotatedPoint = rotQuat * tempPoint;

			rotatedPoint += origin;

			spherePointsComp->push_back(rotatedPoint);*/


			float3 rotatedPoint = rotQuat * float3(sqrt(Pow(newRadius, 2) - Pow(-newRadius + diferenceBetweenSlicesY * i, 2)) * cos(2 * math::pi * j / verSlices), startingPointY.y + diferenceBetweenSlicesY * i, sqrt(Pow(newRadius, 2) - Pow(-newRadius + diferenceBetweenSlicesY * i, 2)) * sin(2 * math::pi * j / verSlices));;

			rotatedPoint += origin;

			spherePointsComp->push_back(rotatedPoint);
		}

	}

	/*float3 tempPointLast = float3(0, newRadius, 0);

	float3 rotatedPointLast = rotQuat * tempPointLast;*/


	float3 rotatedPointLast = rotQuat * float3(0, newRadius, 0);

	rotatedPointLast += origin;

	spherePointsComp->push_back(rotatedPointLast);

}

void RenderManager::CalculateCylinderIndices(std::vector<uint>* cylinderIndicesComp, uint verSlices)
{
	cylinderIndicesComp->clear();

	//Vertical
	for (int i = 0; i < verSlices; i++) {
		cylinderIndicesComp->push_back(i);
		cylinderIndicesComp->push_back(i + verSlices);
	}

	//Horizontal
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < verSlices; j++) {
			if (j == verSlices - 1) {
				cylinderIndicesComp->push_back(i * verSlices + j);
				cylinderIndicesComp->push_back(i * verSlices);
			}
			else {
				cylinderIndicesComp->push_back(i * verSlices + j);
				cylinderIndicesComp->push_back(i * verSlices + j + 1);
			}
		}
	}

}

void RenderManager::CalculateCylinderPoints(PhysBody3D* physBody, std::vector<float3>* cylinderPointsComp, float2 radiusHeight, uint verSlices)
{
	const float radius = radiusHeight.x / 5.f;
	const float height = radiusHeight.y;

	const float3 origin = (float3)physBody->body->getCenterOfMassTransform().getOrigin();
	const float3 startingPointY = float3(0, -(height / 2), 0);
	const float3 endingPointY = float3(0, +(height / 2), 0);


	const btQuaternion rotBtQuat = physBody->body->getCenterOfMassTransform().getRotation();

	float3 rotAxis = (float3)rotBtQuat.getAxis();
	float rotAngle = (float)rotBtQuat.getAngle();
	Quat rotQuat = Quat::RotateAxisAngle(rotAxis, rotAngle);


	cylinderPointsComp->clear();

	//Down
	for (int i = 0; i < verSlices; i++)
	{
		float tempY = startingPointY.y;
		float tempX = radius * cos(2 * math::pi * i / verSlices);
		float tempZ = radius * sin(2 * math::pi * i / verSlices);

		float3 tempPoint = float3(tempX, tempY, tempZ);
		float3 rotatedPoint = rotQuat * tempPoint;

		rotatedPoint += origin;

		cylinderPointsComp->push_back(rotatedPoint);
	}

	//Up
	for (int i = 0; i < verSlices; i++)
	{
		float tempY = endingPointY.y;
		float tempX = radius * cos(2 * math::pi * i / verSlices);
		float tempZ = radius * sin(2 * math::pi * i / verSlices);

		float3 tempPoint = float3(tempX, tempY, tempZ);
		float3 rotatedPoint = rotQuat * tempPoint;

		rotatedPoint += origin;

		cylinderPointsComp->push_back(rotatedPoint);
	}
}

void RenderManager::DrawRayCastLine(float3 pos1, float3 pos2, float4 color, float wireSize)
{

	float3 RaycastPoints[2] = { pos1, pos2 };

	glBindVertexArray(RAYVAO);

	glBindBuffer(GL_ARRAY_BUFFER, RAYVBO);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &RaycastPoints[0], 2 * sizeof(float3));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	localLineShader->shader.Bind();
	localLineShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	localLineShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

	localLineShader->shader.SetFloat4("lineColor", color[0], color[1], color[2], color[3]);

	glLineWidth(wireSize);
	glDrawElements(GL_LINES, rayCastLineIndices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);

	glBindVertexArray(0);

}

void RenderManager::DestroyInstanceRenderers()
{
	_renderMap.clear();
	renderer2D = nullptr;
}

void RenderManager::DrawTransparentMeshes()
{
	OPTICK_EVENT();
	if (_transparencyMeshes.empty())
		return;
	glDepthMask(GL_FALSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;
	for (auto& mesh : _transparencyMeshes)
	{
		Mesh& currentMesh = mesh.second.mesh;
		ResourceMaterial* currentMat = mesh.second.material;
		// Update mesh. If the mesh should draw this frame, call Draw.
		RenderUpdateState renderState = currentMesh.Update();
		if (renderState == RenderUpdateState::DRAW)
		{
			if (currentMat != nullptr && currentMat->material.GetShader() != nullptr)
			{
				currentMat->material.depthDraw = drawDepthIndependent;
				currentMesh.Draw(currentMat->material);
			}
			else
				currentMesh.Draw(Material(), false);
		}
		else if (renderState == RenderUpdateState::SELECTED)
		{
			if (currentMat != nullptr && currentMat->material.GetShader() != nullptr)
				Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&mesh.second); //Selected with Mat
			else
				Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&currentMesh); //Selected without Mat
		}
	}

	glDepthMask(GL_TRUE);
}

void RenderManager::DrawIndependentMeshes()
{
	OPTICK_EVENT();
	if (_independentMeshes.empty())
		return;

	CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;
	for (auto& mesh : _independentMeshes)
	{
		// Do camera culling checks first
		if (currentCamera->isCullingActive)
		{
			if (!currentCamera->IsInsideFrustum(mesh.second.mesh.globalAABB))
			{
				mesh.second.mesh.outOfFrustum = true;
				continue;
			}
			else
				mesh.second.mesh.outOfFrustum = false;
		}
		else if (currentCamera->type != CameraType::SCENE)
		{
			mesh.second.mesh.outOfFrustum = false;
		}

		// Update mesh. If the mesh should draw this frame, call Draw.
		RenderUpdateState renderState = mesh.second.mesh.Update();
		if (renderState == RenderUpdateState::DRAW)
		{
			if (mesh.second.material != nullptr && mesh.second.material->material.GetShader() != nullptr)
			{
				mesh.second.material->material.depthDraw = drawDepthIndependent;
				mesh.second.mesh.Draw(mesh.second.material->material);
			}
			else
				mesh.second.mesh.Draw(Material(), false);
		}
		else if (renderState == RenderUpdateState::SELECTED)
		{
			if (mesh.second.material != nullptr && mesh.second.material->material.GetShader() != nullptr)
				Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&mesh.second); //Selected with Mat
			else
				Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&mesh.second.mesh); //Selected without Mat
		}
	}
	//drawDepthIndependent = !drawDepthIndependent;
}

void RenderManager::DrawTextObjects()
{
	OPTICK_EVENT();
	// Activate Shader to render text
	textRenderingShader->Bind();
	for (auto& textObject : textObjects)
	{
		TextObject text = textObject.second;

		if (!text.draw)
			continue;

		textRenderingShader->SetFloat3v("textColor", &text.color.At(0));
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(TextVAO);

		glDisable(GL_DEPTH_TEST);

		std::string::const_iterator c;
		for (c = text.text.begin(); c != text.text.end(); c++)
		{
			Character ch = FontManager::Characters[*c];

			float scale = text.scale * 0.005f;

			float xPos = text.position.x + ch.Bearing.x * scale;
			float yPos = text.position.y - (ch.Size.y - ch.Bearing.y) * scale;

			float width = ch.Size.x * scale;
			float height = ch.Size.y * scale;

			// TODO: Optimize this by making a single draw call for every 32 characters.
			float vertices[6][4] = {
			  { xPos,     yPos + height,   0.0f, 0.0f },
			  { xPos,     yPos,       0.0f, 1.0f },
			  { xPos + width, yPos,       1.0f, 1.0f },

			  { xPos,     yPos + height,   0.0f, 0.0f },
			  { xPos + width, yPos,       1.0f, 1.0f },
			  { xPos + width, yPos + height,   1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			text.position.x += (ch.Advance >> 6) * scale;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
	}
}
