#include "Headers.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "MeshRenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "InstanceRenderer.h"
#include "ModuleRenderer3D.h"

#include "SkinnedMeshRenderComponent.h"
#include "AnimationComponent.h"

#include "Uniform.h"
#include "Lighting.h"

#define _USE_MATH_DEFINES

#include <math.h>

Mesh::Mesh()
{
	modelMatrix.SetIdentity();
	stencilShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/stencil.shader", 109, "Stencil");
	depthShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/depthMap.shader", 111, "Depth Map (Normal)");
	depthBoneShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/depthMapBone.shader", 113, "Depth Map (Boned)");
}

Mesh::~Mesh()
{
	if (drawPerMeshShader)
	{
		drawPerMeshShader->Dereference();
		drawPerMeshShader = nullptr;
	}
	if (boneMeshShader)
	{
		boneMeshShader->Dereference();
		boneMeshShader = nullptr;
	}
	if (stencilShader)
	{
		stencilShader->Dereference();
		stencilShader = nullptr;
	}
	if (depthShader)
	{
		depthShader->Dereference();
		depthShader = nullptr;
	}
	if (depthBoneShader)
	{
		depthBoneShader->Dereference();
		depthBoneShader = nullptr;
	}
	if (drawPerMesh2D)
	{
		drawPerMesh2D->Dereference();
		drawPerMesh2D = nullptr;
	}
	if (_VAO != 0)
	{
		CleanUp();
	}
	if (&Application::Instance()->renderer3D->renderManager._selectedMesh->mesh == this
		|| Application::Instance()->renderer3D->renderManager._selectedMeshRaw == this)
	{
		Application::Instance()->renderer3D->renderManager.RemoveSelectedMesh();
	}
}

void Mesh::CreateBufferData()
{ 
	_VAO = resource->VAO;
	_VBO = resource->VBO;
	_IBO = resource->IBO;

	if (!is2D)
	{
		drawPerMeshShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/basic.shader", 103, "Basic");
		boneMeshShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/basicBone.shader", 105, "Basic Bone");	}
	else
		drawPerMesh2D = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/basic2D.shader", 110, "Basic2D");
}

void Mesh::Draw(Material material, bool useMaterial)
{
	if (useMaterial) // We use this function to draw the outilne too.
	{
		UniformDraw(material);
	}
	else
	{
		DefaultDraw();
	}

	glBindVertexArray(_VAO);

	glDrawElements(GL_TRIANGLES, _indices->size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	material.UnbindAllTextures();

	if (!useMaterial)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::DefaultDraw()
{
	if (textureID != -1) // Only happens when material components is deactivated.
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	if (is2D)
	{
		drawPerMesh2D->shader.Bind();
		drawPerMesh2D->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);

		float4x4 matrix = modelMatrix.Transposed();

		// Calculate vertex positions:
		// Vertex 1
		float4 vertex1 = { resource->meshInfo.vertices[0].position.x, resource->meshInfo.vertices[0].position.y,0,1 };
		float4 vertex2 = { resource->meshInfo.vertices[1].position.x, resource->meshInfo.vertices[1].position.y,0,1 };
		float4 vertex3 = { resource->meshInfo.vertices[2].position.x, resource->meshInfo.vertices[2].position.y,0,1 };
		float4 vertex4 = { resource->meshInfo.vertices[3].position.x, resource->meshInfo.vertices[3].position.y,0,1 };

		vertex1 = matrix * vertex1;
		vertex2 = matrix * vertex2;
		vertex3 = matrix * vertex3;
		vertex4 = matrix * vertex4;

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		// Convert NDC coordinates to window coordinates
		float2 windowVertex1 = float2((vertex1.x + 1.0f) * 0.5f * viewport[2], (vertex1.y + 1.0f) * 0.5f * viewport[3]);
		float2 windowVertex2 = float2((vertex2.x + 1.0f) * 0.5f * viewport[2], (vertex2.y + 1.0f) * 0.5f * viewport[3]);
		float2 windowVertex3 = float2((vertex3.x + 1.0f) * 0.5f * viewport[2], (vertex3.y + 1.0f) * 0.5f * viewport[3]);
		float2 windowVertex4 = float2((vertex4.x + 1.0f) * 0.5f * viewport[2], (vertex4.y + 1.0f) * 0.5f * viewport[3]);

		float quadWidth = abs(windowVertex2.x - windowVertex1.x);
		float quadHeight = abs(windowVertex3.y - windowVertex1.y);

		float2 quadDimensions = { quadWidth, quadHeight };
		float2 quadPositions = { windowVertex1.x, windowVertex1.y};


		drawPerMesh2D->shader.SetFloat2v("normalizedPosition", quadPositions.ptr());
		drawPerMesh2D->shader.SetFloat2v("normalizedSize", quadDimensions.ptr());
		drawPerMesh2D->shader.SetFloat("limit", opacityLimit);
		drawPerMesh2D->shader.SetInt("opacityDir", (int)opacityDir);
		drawPerMesh2D->shader.SetFloat("totalOpacity", opacity);

		return;
	}

	if (component->_hasBones)
	{
		boneMeshShader->shader.Bind();
		boneMeshShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
		boneMeshShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
		boneMeshShader->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);

		//boneMeshShader->shader.SetInt("diffuseTexture", textureID);

		SkinnedMeshRenderComponent* smComp = (SkinnedMeshRenderComponent*)component;

		if (!smComp->hasAnim)
		{
			smComp->UpdateBones();
		}

		for (int i = 0; i < smComp->goBonesArr.size(); ++i)
		{
			boneMeshShader->shader.SetMatFloat4v("finalBonesMatrices[" + std::to_string(i) + "]", &smComp->goBonesArr[i].Transposed().v[0][0]);
		}

	}
	else if (!is2D)
	{
		drawPerMeshShader->shader.Bind();
		drawPerMeshShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
		drawPerMeshShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
		drawPerMeshShader->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);
		
		//drawPerMeshShader->shader.SetInt("diffuseTexture", textureID);
	}

}

void Mesh::UniformDraw(Material material)
{
	if (material.depthDraw)
	{
		if (component->_hasBones)
		{	
			return;
		}
		depthShader->shader.Bind();
		depthShader->shader.SetMatFloat4v("dirLightSpaceMatrix",
			&Lighting::GetLightMap().directionalLight.lightSpaceMatrix.v[0][0]);
		depthShader->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);

		return;
	}
	//Update the material uniforms
	material.Update(Application::Instance()->camera->currentDrawingCamera->GetViewMatrix(),
		Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix(),
		&modelMatrix.v[0][0]);

	//Update only the bones
	if (component->_hasBones)
	{
		SkinnedMeshRenderComponent* smComp = (SkinnedMeshRenderComponent*)component;

		if (!smComp->hasAnim)
		{
			smComp->UpdateBones();
		}

		material.UpdateBones(smComp->goBonesArr);
	}

	material.GetShader()->shader.SetFloat4v("ColourTest", color.ptr());
}

void Mesh::StencilDraw()
{
	stencilShader->shader.Bind();
	stencilShader->shader.SetFloat("outlineSize", 1.04f);
	stencilShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	stencilShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	stencilShader->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);
}

RenderUpdateState Mesh::Update()
{
	if (!draw)
		return RenderUpdateState::NODRAW;

	if (showVertexNormals)
		Application::Instance()->renderer3D->renderManager.DrawVertexNormals(this);
	if (showFaceNormals)
		Application::Instance()->renderer3D->renderManager.DrawFaceNormals(this);
	if (showAABB)
		Application::Instance()->renderer3D->renderManager.DrawAABB(this);
	if (showOBB)
		Application::Instance()->renderer3D->renderManager.DrawOBB(this);

	if (outOfFrustum && !is2D)
		return RenderUpdateState::NODRAW;

	if (component && component->_gameObject->isSelected)
	{
		//if (isIndependent)
		//	return false; // We dont want to render this object twice when selected.
		//else
		//{
			//Application::Instance()->renderer3D->renderManager.SetSelectedMesh(this);
		if (textureID != -1.0f)
		{
			OpenGLTextureID = TextureManager::BindTexture(textureID);
		}
			return RenderUpdateState::SELECTED;
		//}
	}
	if (isIndependent) // We dont use the TextureManager to set independent meshes's textures.
		return RenderUpdateState::DRAW;
	
	if (textureID != -1.0f)
	{
		OpenGLTextureID = TextureManager::BindTexture(textureID);
	}

	return RenderUpdateState::DRAW;
}

void Mesh::DrawAsSelected(Material material, uint materialID)
{
	//// TODO: Do this inside ModuleRender3D, and allow to enable and disable it.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	// Draw model normal size
	if (isIndependent)
	{
		Draw(material);
	}
	else
	{
		/*InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(component->_meshID + materialID, 0);
		manager->DrawInstance(this);*/
	}

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

#ifdef STANDALONE
	/*stencilShader->shader.Bind();
	stencilShader->shader.SetFloat("outlineSize", 0.04f);
	stencilShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
	stencilShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
	stencilShader->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);*/
#endif
	// Draw model bigger size using the stencilShader
	/*if (isIndependent)
	{*/
		//Draw(material, false);
		StencilDraw();
	/*}
	else
	{
		InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(component->_meshID, 0);
		manager->DrawInstance(this, false);
	}*/

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Mesh::DrawAsSelected()
{
	//// TODO: Do this inside ModuleRender3D, and allow to enable and disable it.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	if (isIndependent)
	{
		Draw(Material(), false);
	}
	/*InstanceRenderer* manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(component->_meshID, 0);
	manager->DrawInstance(this);*/

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

//#ifdef STANDALONE
//	stencilShader->shader.Bind();
//	stencilShader->shader.SetFloat("outlineSize", 0.04f);
//	stencilShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
//	stencilShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
//	stencilShader->shader.SetMatFloat4v("model", &modelMatrix.v[0][0]);
//#endif
	
	StencilDraw();
	/*manager = Application::Instance()->renderer3D->renderManager.GetRenderManager(component->_meshID, 0);
	manager->DrawInstance(this, false);*/

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Mesh::InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	this->_vertices = new std::vector<Vertex>(vertices);
	this->_indices = new std::vector<uint>(indices);
}

void Mesh::InitWithResource(ResourceMesh* res)
{
	resource = res;
	this->_vertices = &res->meshInfo.vertices;
	this->_indices = &res->meshInfo.indices;
}

void Mesh::CleanUp()
{	
	//RELEASE(_vertices);
	//RELEASE(_indices);
}

void Mesh::CalculateBoundingBoxes()
{
	globalOBB = localAABB;
	globalOBB.Transform(modelMatrix.Transposed());

	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(globalOBB);
}
