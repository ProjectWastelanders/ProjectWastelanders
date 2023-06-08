#include "Headers.h"
#include "ParticleSystemComponent.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"
#include "ParticleManager.h"
#include "LayerEditor.h"
#include "P_MainModule.h"
#include "P_EmissionModule.h"
#include "P_ShapeModule.h"
#include "P_CircleShape.h"
#include "P_RectangleShape.h"
#include "P_SphereShape.h"
#include "P_CubeShape.h"
#include "P_ConeShape.h"
#include "BillBoardComponent.h"



ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject) : Component(gameObject)
{
	_type = Type::PARTICLE_SYSTEM;
	_resource = nullptr;
	app = Application::Instance();

	ParticleEmitter.component = this;

	//Initialize Particle System Modules
	P_Module* mainModule = (P_Module*)new P_MainModule();
	mainModule->component = this;
	ParticleModules.push_back(mainModule);

	P_Module* emissionModule = (P_Module*)new P_EmissionModule();
	emissionModule->component = this;
	ParticleModules.push_back(emissionModule);

	P_Module* shapeModule = (P_Module*)new P_ShapeModule();
	shapeModule->component = this;
	ParticleModules.push_back(shapeModule);

	_gameObject->AddComponentOfType(Type::BILLBOARD);

	//Default Particle
	particleProps.position = _gameObject->transform->GetGlobalPosition();
	//particleProps.rot = float3::zero;
	particleProps.startsize = float3::one;
	particleProps.endsize = float3::zero;
	particleProps.speed = float3(0.0f, 1.0f, 0.0f);
	particleProps.rotation = float3(0.0f, 0.0f, 360.0f);
	particleProps.acceleration = float3(1.0f, 1.0f, 1.0f);
	particleProps.speedVariation = float3(1.0f, 1.0f, 1.0f);
	particleProps.startColor = float4(255.0f, 255.0f, 255.0f, 1.0f); //r g b a
	particleProps.endColor = float4(255.0f, 255.0f, 255.0f, 1.0f); //r g b a

	particleProps.Lifetime = 5.0f;

	ParticleEmitter.SetParticlePoolSize(size);
}

ParticleSystemComponent::ParticleSystemComponent(GameObject* gameObject, ParticleSystemComponent& copy) : Component(gameObject)
{
	_type = Type::PARTICLE_SYSTEM;
	_resource = nullptr;
	app = Application::Instance();

	ParticleEmitter.component = this;

	//Initialize Particle System Modules
	P_Module* mainModule = (P_Module*)new P_MainModule();
	mainModule->component = this;
	ParticleModules.push_back(mainModule);

	P_Module* emissionModule = (P_Module*)new P_EmissionModule();
	emissionModule->component = this;
	ParticleModules.push_back(emissionModule);

	P_Module* shapeModule = (P_Module*)new P_ShapeModule();
	shapeModule->component = this;
	ParticleModules.push_back(shapeModule);

	BillBoardComponent* billboard = (BillBoardComponent*)_gameObject->AddComponentOfType(Type::BILLBOARD);
	BillBoardComponent* copyBB = copy._gameObject->GetComponent<BillBoardComponent>();
	if (copyBB != nullptr)
		billboard->typeofBBoard = copyBB->typeofBBoard;

	//Default Particle
	particleProps.position = _gameObject->transform->GetGlobalPosition();
	//particleProps.rot = float3::zero;
	particleProps.startsize = copy.particleProps.startsize;
	particleProps.endsize = copy.particleProps.endsize;
	particleProps.speed = copy.particleProps.speed;
	particleProps.acceleration = copy.particleProps.acceleration;
	particleProps.speedVariation = copy.particleProps.speedVariation;
	particleProps.startColor = copy.particleProps.startColor; //r g b a
	particleProps.endColor = copy.particleProps.endColor; //r g b a

	particleProps.Lifetime = copy.particleProps.Lifetime;

	ParticleEmitter.loop = copy.ParticleEmitter.loop;
	ParticleEmitter.stop = copy.ParticleEmitter.stop;
	ParticleEmitter.playOnAwake = copy.ParticleEmitter.playOnAwake;
	ParticleEmitter.StartDelay = copy.ParticleEmitter.StartDelay;
	ParticleEmitter.Duration = copy.ParticleEmitter.Duration;
	ParticleEmitter.enableEmissionModule = copy.ParticleEmitter.enableEmissionModule;
	ParticleEmitter.ParticlesPerSecond = copy.ParticleEmitter.ParticlesPerSecond;

	ParticleEmitter.SetParticlePoolSize(size);

	CreateEmitterMesh(copy._resource->UID);
	if (copy._resourceText != nullptr)
		ChangeEmitterMeshTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(copy._resourceText->UID));
}

ParticleSystemComponent::~ParticleSystemComponent()
{
	for (int i = 0; i < ParticleModules.size(); i++)
	{
		RELEASE(ParticleModules[i]);
	}
	ParticleModules.clear();

	if (_resourceText != nullptr)
		_resourceText->Dereference();

	BillBoardComponent* billBoard = _gameObject->GetComponent<BillBoardComponent>();
	_gameObject->DestroyComponent(billBoard);
}

void ParticleSystemComponent::CreateEmitterMesh(uint resourceUID)
{

	if (_resource != nullptr)
	{
		DestroyEmitterMesh();

		_resource->Dereference();
		_resourceUID = _resource->UID;
		_resource = nullptr;
	}

	_resource = (ResourceMesh*)ModuleResourceManager::S_LoadResource(resourceUID);

	if (_resource == nullptr)
	{
		ParticleEmitter._meshID = -1;
		return;
	}

	ParticleEmitter._meshID = resourceUID;
	ParticleEmitter.manager = app->renderer3D->renderManager.GetRenderManager(resourceUID,0);
	ParticleEmitter.manager->isParticle = true;

	for (Particle& var : ParticleEmitter.ParticleList)
	{
		var._instanceID = Application::Instance()->renderer3D->renderManager.AddMeshParticle(_resource);
		
		//This line is needed because when you add mesh into the rendermanager it will be drawn,
		//when we are at this point we don't want to draw the mesh of the particle till the engine is playing
		Mesh& meshRef = Application::Instance()->renderer3D->renderManager.GetRenderManager(resourceUID, 0)->GetMap()[var._instanceID].mesh;
		
		meshRef.draw = false;
		if (ParticleEmitter.isParticleAnimated)
		{
			meshRef.isParticleAnim = true;
		}
		else
		{
			meshRef.isParticleAnim = false;
		}
	}

}

Mesh& ParticleSystemComponent::GetEmitterMesh()
{
	for (int i = 0; i < ParticleEmitter.ParticleList.size(); i++)
	{
		if (ParticleEmitter._meshID != -1)
		{
			Mesh& temp = ParticleEmitter.manager->GetMap()[ParticleEmitter.ParticleList[i]._instanceID].mesh;

			//Fa falta guardar la posici� de cada mesh ?

			return temp;
		}
	}
	// TODO: insert return statement here
}

void ParticleSystemComponent::OnEnable()
{
}

void ParticleSystemComponent::OnDisable()
{
}

void ParticleSystemComponent::DestroyEmitterMesh()
{
	if (ParticleEmitter._meshID == -1)
		return;


	InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(ParticleEmitter._meshID, false);
	if (manager != nullptr)
	{

		for (Particle& var : ParticleEmitter.ParticleList)
		{
			manager->GetMap().erase(var._instanceID);
		}

	}

	ParticleEmitter._meshID = -1;
}

void ParticleSystemComponent::ChangeEmitterMeshTexture(ResourceTexture* resource)
{
	if (resource == nullptr)
	{
		ParticleEmitter.emitterTexture._textureID = -1.0f;
		_resourceText = nullptr;

		return;
	}

	ParticleEmitter.emitterTexture._textureID = resource->OpenGLID;

	if (_resourceText != nullptr)
		_resourceText->Dereference();

	_resourceText = resource;

	for (Particle& var : ParticleEmitter.ParticleList)
	{
		var.texture = ParticleEmitter.emitterTexture;
	}

	//if (resource->isTransparent && !isUI)
	//	meshRenderer->ChangeMeshRenderType(MeshRenderType::TRANSPARENCY);

}

#ifdef STANDALONE

void ParticleSystemComponent::OnEditor()
{
	bool created = true;
	if (ImGui::CollapsingHeader("Particle System", &created, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play"))
		{
			if (!pauseOnScene)
			{
				ResetEmitterTimers();
			}
			if (ParticleEmitter.Duration > 0 || ParticleEmitter.loop)
			{
				StopEmittingOnGame = false;
				SetPlayOnScene(true);
				SetPauseOnScene(false);
				
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			if (ParticleEmitter.Duration > 0 || ParticleEmitter.loop)
			{
				SetPauseOnScene(true);
				SetPlayOnScene(false);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop Emitting"))
		{
			ResetEmitterTimers();
			StopEmittingOnGame = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			StopEmitter();
		}
		if (ImGui::DragInt("Particle Vector Size", &size, 1.0f, 1, 10000))
		{
			if (!LayerGame::S_IsPlaying() && !GetPlayOnScene() && !GetPauseOnScene())
			{
				ParticleEmitter.SetParticlePoolSize(size);
				sizeCpy = size;
				//ChangeEmitterMeshTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(_resourceTextUID));
			}
			else
			{
				size = sizeCpy;
				std::string popUpmessage = "This Option only works when the emitter is stopped. Please Stop the emitter first";
				LayerEditor::S_AddPopUpMessage(popUpmessage);
			}
		}

		if (ParticleEmitter._meshID == -1)
		{
			ImGui::Checkbox("Particle Animated", &ParticleEmitter.isParticleAnimated);

			ImGui::TextWrapped("First ensure yourself that if you want an animated particle the checkbox above is marked");
			ImGui::TextWrapped("No mesh loaded! Drag an .hmesh file below to load a mesh ");

			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag .hmesh here"); ImGui::SameLine();

			// Create Droped mesh
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
				{
					const uint* drop = (uint*)payload->Data;

					CreateEmitterMesh(*drop);

					std::string popUpmessage = "Loaded Mesh: ";
					LayerEditor::S_AddPopUpMessage(popUpmessage);

				}
				ImGui::EndDragDropTarget();
			}
			ImGui::HelpMarker("You can find .hmesh files by clicking on any model file (FBX or DAE). They will appear below the file icon in the Project window.");

			if (ImGui::Button("Set Plane as Emitter Mesh"))
			{
				CreateEmitterMesh(app->renderer3D->renderManager.planeUID);
			}
		}
		else
		{
			if (ImGui::Button("Delete Emitter Mesh"))
			{


				StopEmitter();

				DestroyEmitterMesh();

				DestroyEmitterMeshTexture();

				std::string popUpmessage = "Mesh in the emitter Destroyed ";
				LayerEditor::S_AddPopUpMessage(popUpmessage);

			}

			std::string imageName;
			int width = 0;
			int height = 0;
			if (ParticleEmitter.emitterTexture._textureID != -1.0f && _resourceText != nullptr)
			{
				ImGui::Image((ImTextureID)(uint)ParticleEmitter.emitterTexture._textureID, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

				imageName = _resourceText->debugName;
				width = _resourceText->width;
				height = _resourceText->height;
			}
			else
			{
				ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
				imageName = "None";
			}
			if (ParticleEmitter.emitterTexture._textureID != -1 && _resourceText && _resourceText->isTransparent && ParticleEmitter.isParticleAnimated)
			{
				if (ImGui::DragInt("Num of Rows in Atlas", &ParticleEmitter.emitterTexture.numOfRows, 1.0f, 1, 12))
				{
					if (ParticleEmitter.isParticleAnimated)
					{
						for (Particle& var : ParticleEmitter.ParticleList)
						{
							var.texture = ParticleEmitter.emitterTexture;
						}
					}
				}
			}

			if (ParticleEmitter.emitterTexture._textureID == -1)
			{
				ImGui::TextWrapped("No texture loaded! Drag an .htext file below to load a texture ");

				ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag .htext here"); ImGui::SameLine();

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
					{
						//Drop asset from Asset window to scene window
						const uint* drop = (uint*)payload->Data;

						ResourceTexture* resource = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);

						ChangeEmitterMeshTexture(resource);

						std::string popUpmessage = "Loaded Texture: ";
						LayerEditor::S_AddPopUpMessage(popUpmessage);
					}
					ImGui::EndDragDropTarget();
				}

			}
			else
			{
				if (_resourceText)
				{
					if (ImGui::Checkbox("Transparent", &_resourceText->isTransparent))
					{
						std::string popUpmessage = "Texture in the emitter set as transparent";
						LayerEditor::S_AddPopUpMessage(popUpmessage);
					}
				}

				if (ImGui::Button("Delete Emitter Texture"))
				{

					DestroyEmitterMeshTexture();

					std::string popUpmessage = "Texture in the emitter Destroyed ";
					LayerEditor::S_AddPopUpMessage(popUpmessage);

				}
			}
			ImGui::NewLine();
			for (int i = 0; i < ParticleModules.size(); i++)
			{
				ParticleModules[i]->OnEditor();
			}
		}
		}

	if (!created)
		this->_gameObject->DestroyComponent(this);
}

void ParticleSystemComponent::MarkAsDead()
{
	if (ParticleEmitter._meshID != -1)
	{
		DestroyEmitterMesh();

		if (_resource != nullptr)
		{
			_resource->Dereference();
			_resourceUID = _resource->UID;
			_resource = nullptr;
		}

		app->renderer3D->particleManager.RemoveEmitterInList(&ParticleEmitter);
	}

	if (_resourceText != nullptr)
	{
		_resourceText->Dereference();
		_resourceTextUID = _resourceText->UID;
		_resourceText = nullptr;
	}
}

void ParticleSystemComponent::MarkAsAlive()
{
	CreateEmitterMesh(_resourceUID);

	ChangeEmitterMeshTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(_resourceTextUID));
}
#endif

void ParticleSystemComponent::ResetEmitterTimers()
{
	ParticleEmitter.StartDelay = ParticleEmitter.StartDelayCpy;
	ParticleEmitter.Duration = ParticleEmitter.DurationCpy;
}

void ParticleSystemComponent::DestroyEmitterMeshTexture()
{
	if (_resourceText != nullptr)
	{
		ParticleEmitter.emitterTexture._textureID = -1.0f;
		_resourceText = nullptr;

		for (Particle& var : ParticleEmitter.ParticleList)
		{
			var.texture = ParticleEmitter.emitterTexture;
		}

	}
}

void ParticleSystemComponent::StopEmitter()
{
	if (GetPlayOnScene())
	{
		SetPlayOnScene(false);
		SetPauseOnScene(false);
		if (!LayerGame::S_IsPlaying()) {
			if (ParticleEmitter.StartDelay <= 0)
			{
				ParticleEmitter.ResetEmitter();
			}
			else
			{
				ParticleEmitter.StartDelay = ParticleEmitter.StartDelayCpy;
			}
		}
	}
}

void ParticleSystemComponent::Serialization(json& j)
{
	json _j;

	_j["Type"] = _type;

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

	if (_resourceText != nullptr)
	{
		_j["ResourceTextUID"] = _resourceText->UID;
	}
	else
	{
		_j["ResourceTextUID"] = 0;
	}

	if (ParticleModules.empty() == false)
	{
		_j["ParticleModules"]["ModuleMain"]["BeginScale"] = { particleProps.startsize.x, particleProps.startsize.y, particleProps.startsize.z };
		_j["ParticleModules"]["ModuleMain"]["EndScale"] = { particleProps.endsize.x, particleProps.endsize.y, particleProps.endsize.z };
		_j["ParticleModules"]["ModuleMain"]["Speed"] = { particleProps.speed.x, particleProps.speed.y, particleProps.speed.z };
		_j["ParticleModules"]["ModuleMain"]["SpeedVariation"] = { particleProps.speedVariation.x, particleProps.speedVariation.y, particleProps.speedVariation.z };
		_j["ParticleModules"]["ModuleMain"]["acceleration"] = { particleProps.acceleration.x, particleProps.acceleration.y, particleProps.acceleration.z };
		_j["ParticleModules"]["ModuleMain"]["rotation"] = { particleProps.rotation.x, particleProps.rotation.y, particleProps.rotation.z };
		_j["ParticleModules"]["ModuleMain"]["LifeTime"] = particleProps.Lifetime;
		_j["ParticleModules"]["ModuleMain"]["Duration"] = ParticleEmitter.Duration;
		_j["ParticleModules"]["ModuleMain"]["Delay"] = ParticleEmitter.StartDelay;
		_j["ParticleModules"]["ModuleMain"]["Looping"] = ParticleEmitter.loop;	
		_j["ParticleModules"]["ModuleMain"]["PlayOnAwake"] = ParticleEmitter.playOnAwake;
		_j["ParticleModules"]["ModuleEmission"]["ParticlesPerSecond"] = ParticleEmitter.ParticlesPerSecond;
		_j["ParticleModules"]["ModuleEmission"]["Enable"] = ParticleEmitter.enableEmissionModule;

		if (GetCurrentShape() == nullptr) {
			_j["ParticleModules"]["ShapeModule"]["ShapeType"] = ShapeType::NONE;
		}
		else {
			_j["ParticleModules"]["ShapeModule"]["ShapeType"] = GetCurrentShape()->shapeType;
			GetCurrentShape()->Serialization(_j);
		}		
	}
	_j["IsParticleAnimated"] = ParticleEmitter.isParticleAnimated;
	_j["ParticleVectorSize"] = size;
	_j["Enabled"] = _isEnabled;

	j["Components"].push_back(_j);
}

void ParticleSystemComponent::DeSerialization(json& j)
{
	ResourceModel* model = nullptr;
	if (ModuleResourceManager::resources.count(j["ModelUID"]) != 0)
		model = (ResourceModel*)ModuleResourceManager::resources[j["ModelUID"]];

	uint savedUID = j["ResourceTextUID"];

	ResourceTexture* resourcetext = savedUID == 0 ? nullptr : (ResourceTexture*)ModuleResourceManager::S_LoadResource(j["ResourceTextUID"]);

	if (model == nullptr)
	{
		Console::S_Log("A scene mesh render data was not found.");
		return;
	}

	uint index = j["Index inside model"];
	if (index < model->modelMeshes.size())
	{
		ResourceMesh* resourceMesh = model->modelMeshes[index];

		CreateEmitterMesh(resourceMesh->UID);

	}

	if (resourcetext != nullptr)
	{
		ChangeEmitterMeshTexture(resourcetext);
	}

	std::vector<float> tempstartsize = j["ParticleModules"]["ModuleMain"]["BeginScale"];
	particleProps.startsize = { tempstartsize[0],tempstartsize[1],tempstartsize[2] };
	std::vector<float> tempendsize = j["ParticleModules"]["ModuleMain"]["EndScale"];
	particleProps.endsize = { tempendsize[0],tempendsize[1],tempendsize[2] };
	std::vector<float> tempspeed = j["ParticleModules"]["ModuleMain"]["Speed"];
	particleProps.speed = { tempspeed[0],tempspeed[1],tempspeed[2] };
	std::vector<float> tempspeedVariation = j["ParticleModules"]["ModuleMain"]["SpeedVariation"];
	particleProps.speedVariation = { tempspeedVariation[0],tempspeedVariation[1],tempspeedVariation[2] };
	std::vector<float> tempacceleration = j["ParticleModules"]["ModuleMain"]["acceleration"];
	particleProps.acceleration = { tempacceleration[0],tempacceleration[1],tempacceleration[2] };
	//std::vector<float> temprotation = j["ParticleModules"]["ModuleMain"]["rotation"];
	//particleProps.rotation = { temprotation[0],temprotation[1],temprotation[2] };
	particleProps.Lifetime = j["ParticleModules"]["ModuleMain"]["LifeTime"];
	ParticleEmitter.Duration = j["ParticleModules"]["ModuleMain"]["Duration"];
	ParticleEmitter.DurationCpy = ParticleEmitter.Duration;
	ParticleEmitter.StartDelay = j["ParticleModules"]["ModuleMain"]["Delay"];
	ParticleEmitter.playOnAwake = j["ParticleModules"]["ModuleMain"]["PlayOnAwake"];
	ParticleEmitter.loop = j["ParticleModules"]["ModuleMain"]["Looping"];
	ParticleEmitter.ParticlesPerSecond = j["ParticleModules"]["ModuleEmission"]["ParticlesPerSecond"];
	ParticleEmitter.enableEmissionModule = j["ParticleModules"]["ModuleEmission"]["Enable"];
	size = j["ParticleVectorSize"];
	sizeCpy = j["ParticleVectorSize"];
	//ParticleEmitter.manager->isParticleAnimated = j["IsParticleAnimated"];

	ParticleEmitter.SetParticlePoolSize(size);

	ShapeType shape = ShapeType::NONE;

	if (j.contains("ParticleModules") && j["ParticleModules"].contains("ShapeModule") 
		&& j["ParticleModules"]["ShapeModule"].contains("ShapeType"))
	{
		shape = j["ParticleModules"]["ShapeModule"]["ShapeType"];
		CreateCurrentShape(shape);
	}

	if (shape != ShapeType::NONE) 
	{
		GetCurrentShape()->DeSerialization(j);
	}

	bool enabled = j["Enabled"];
}

void ParticleSystemComponent::SetPlayOnGame(bool playongame)
{
	this->playOnGame = playongame;
}

void ParticleSystemComponent::SetStopEmitting(bool stopemitting)
{
	this->StopEmittingOnGame = stopemitting;
}

void ParticleSystemComponent::SetPlayOnScene(bool playonscene)
{
	this->playOnScene = playonscene;
}

void ParticleSystemComponent::SetPauseOnScene(bool pauseonscene)
{
	this->pauseOnScene = pauseonscene;
}

void ParticleSystemComponent::CreateCurrentShape(ShapeType type)
{
	switch (type) {
	case ShapeType::NONE:
		return;
		break;
	case ShapeType::CIRCLE:
	{
		P_Module* circleShape = (P_Module*)new P_CircleShape();
		circleShape->component = this;
		ParticleModules.push_back(circleShape);
	}
	break;
	case ShapeType::RECTANGLE:
	{
		P_Module* rectangleShape = (P_Module*)new P_RectangleShape();
		rectangleShape->component = this;
		ParticleModules.push_back(rectangleShape); 
	}
	break;
	case ShapeType::SPHERE:
	{
		P_Module* sphereShape = (P_Module*)new P_SphereShape();
		sphereShape->component = this;
		ParticleModules.push_back(sphereShape);
	}
	break;
	case ShapeType::CUBE:
	{
		P_Module* cubeShape = (P_Module*)new P_CubeShape();
		cubeShape->component = this;
		ParticleModules.push_back(cubeShape);
	}
	break;
	case ShapeType::CONE:
	{
		P_Module* coneShape = (P_Module*)new P_ConeShape();
		coneShape->component = this;
		ParticleModules.push_back(coneShape);
	}
	break;
	}
}

P_ShapeModule* ParticleSystemComponent::GetCurrentShape()
{
	for (int i = 0; i < ParticleModules.size(); i++)
	{
		if (ParticleModules[i]->type == P_ModuleType::SHAPE)
		{
			if (ParticleModules[i]->shapeType != ShapeType::NONE) {				
				return (P_ShapeModule*)ParticleModules[i];
			}
		}
	}

	return nullptr;
}

void ParticleSystemComponent::DeleteCurrentShape()
{
	for (int i = 0; i < ParticleModules.size(); i++)
	{
		if (ParticleModules[i]->type == P_ModuleType::SHAPE)
		{
			if (ParticleModules[i]->shapeType != ShapeType::NONE) {
				RELEASE(ParticleModules[i]);
				ParticleModules.erase(ParticleModules.begin() + i);
				break;
			}
		}
	}
}
