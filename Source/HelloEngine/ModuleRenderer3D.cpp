#include "Headers.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "SDL_opengl.h"
#include "XMLNode.h"
#include "ModuleXML.h"
#include "ModuleLayers.h"
#include "MeshRenderComponent.h"
#include "LayerEditor.h"

#include "Emitter.h"
#include "ParticleSystemComponent.h"
#include "Lighting.h"

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{

}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	_cameras = app->camera;
	
	Console::S_Log("Initializing OpenGL 4.1");

	//Create context
	context = SDL_GL_CreateContext(ModuleWindow::window);

	Console::S_Log("Initializing Glew.");

	GLenum er = glewInit();

	SDL_GL_MakeCurrent(ModuleWindow::window, context);

	//Use Vsync
	XMLNode renderNode = app->xml->GetConfigXML().FindChildBreadth("renderer");
	isVSync = renderNode.node.child("vsync").attribute("value").as_bool();
	ToggleVSync(isVSync);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);

	// Enable opacity
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	renderManager.Init();

	// Projection matrix for

	OnResize(ModuleWindow::width, ModuleWindow::height);

	isRenderingColliders = true;

	return ret;
}

// PreUpdate: clear buffer
UpdateStatus ModuleRenderer3D::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleRenderer3D::DrawGame()
{
	if (_cameras->activeGameCamera != nullptr && _cameras->activeGameCamera->active)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind to default buffer because a camera buffer is not necessary with only one display.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		_cameras->currentDrawingCamera = _cameras->activeGameCamera;

		renderManager.Draw();
		particleManager.Draw();
		// Draw all 2D meshes.
		renderManager.Draw2D();
	}
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRenderer3D::PostUpdate()
{
#ifdef STANDALONE

	//SCENE RENDERING
	if (app->layers->editor->S_GetWindowActive(ImWindowID::SCENE) &&
		_cameras->sceneCamera->active)
	{
		_cameras->currentDrawingCamera = _cameras->sceneCamera;
		
		ShadowRenderPass();

		//Normal Rendering of scene
		_cameras->sceneCamera->frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			//RENDER SCENE
			ModuleLayers::S_DrawLayers();
			particleManager.Draw();
			renderManager.Draw();
			renderManager.DrawDebug();
			_cameras->DrawCameraFrustums();
	}

	//CANVAS RENDERING
	if (app->layers->editor->S_GetWindowActive(ImWindowID::UI) &&
		_cameras->UICamera->active)
	{
		_cameras->UICamera->frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		_cameras->currentDrawingCamera = _cameras->UICamera;

		particleManager.Draw();
		renderManager.Draw2D();

	}

	//ImWin GAME RENDERING
	if (app->layers->editor->S_GetWindowActive(ImWindowID::GAME) && 
		_cameras->activeGameCamera != nullptr && _cameras->activeGameCamera->active)
	{
		_cameras->currentDrawingCamera = _cameras->activeGameCamera;

		ShadowRenderPass();

		_cameras->activeGameCamera->frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		particleManager.Draw();
		renderManager.Draw();

		// Draw all 2D meshes.
		renderManager.Draw2D();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ModuleLayers::S_DrawEditor();
#else
	DrawGame();
#endif

	SDL_GL_SwapWindow(ModuleWindow::window);

	return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleRenderer3D::ShadowRenderPass()
{
	glCullFace(GL_FRONT);
	//Depth map (Shadow)
	_cameras->currentDrawingCamera->frameBuffer.BindShadowBuffer();
	//RENDER SCENE
	renderManager.Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);

	glViewport(0, 0, ModuleWindow::width, ModuleWindow::height);
	/*if (hasShadows)
	{*/
		Lighting::GetLightMap().shadowMap = _cameras->currentDrawingCamera->frameBuffer.GetDepthTexture();
	/*}
	else
	{
		Lighting::GetLightMap().shadowMap = 1;
	}*/
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	XMLNode configNode = app->xml->GetConfigXML();

	configNode.node.child("renderer").child("vsync").attribute("value").set_value(isVSync);

	configNode.Save();

	LOG("Deleting context");

	SDL_GL_DeleteContext(context);

	LOG("Context deleted");

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	_cameras->RequestFrameBufferRegen(_cameras->sceneCamera, width, height);
	_cameras->RequestFrameBufferRegen(_cameras->UICamera, width, height);

	if (_cameras->activeGameCamera != nullptr)
		_cameras->RequestFrameBufferRegen(_cameras->activeGameCamera, width, height);

	ModuleWindow::width = width;
	ModuleWindow::height = height;
}

void ModuleRenderer3D::ToggleVSync(bool isOn)
{
	isVSync = isOn;
	SDL_GL_SetSwapInterval(isVSync);
}

void ModuleRenderer3D::ToggleOpenGLSystem(bool enable, int openGLMacro)
{
	if (enable) glEnable(openGLMacro);
	else glDisable(openGLMacro);
}

void ModuleRenderer3D::ToggleOpenGLWireframe(bool enable)
{
	if (enable)glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GameObject* ModuleRenderer3D::RaycastFromMousePosition(LineSegment& ray, CameraObject* camera)
{
	auto& gameObjects = ModuleLayers::gameObjects;
	bool hit = false;
	std::vector<uint> hitGameobjects;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject.second == nullptr)
			continue;

		if (!gameObject.second->_isActive || gameObject.second->_isPendingToDelete || gameObject.second->_isDestroyed)
			continue;
		if (!gameObject.second->HasComponent<MeshRenderComponent>())
			continue;
		
		MeshRenderComponent* aux = gameObject.second->GetComponent<MeshRenderComponent>();
		if (aux->GetResourceUID() <= 0)
			continue;
		if (ray.Intersects(aux->GetMesh().globalAABB))
		{
			hitGameobjects.push_back(gameObject.first);
		}
	}

	if (hitGameobjects.empty())
		return nullptr;

	std::multimap<float, uint> orderedHits;

	// Find closest game object.
	for (int i = 0; i < hitGameobjects.size(); i++)
	{
		float distance = gameObjects[hitGameobjects[i]]->transform->GetGlobalMatrix().TranslatePart().DistanceSq(camera->cameraFrustum.pos);
		orderedHits.emplace(std::make_pair(distance, hitGameobjects[i]));
	}

	// Check every hit gameObject in closest to furthest order.
	for (auto& hit : orderedHits)
	{
		// Create triangles for every three vertices.
		MeshRenderComponent* hitMesh = gameObjects[hit.second]->GetComponent<MeshRenderComponent>();
		std::vector<Vertex> meshVertices = hitMesh->GetMeshVertices();
		std::vector<uint> meshIndices = hitMesh->GetMeshIndices();

		int faceNum = meshIndices.size() / 3;
		std::vector<Triangle> triangles;
		int j = 0;
		for (int i = 0; i < faceNum; i++)
		{
			triangles.emplace_back(meshVertices[meshIndices[j++]].position, meshVertices[meshIndices[j++]].position, meshVertices[meshIndices[j++]].position);
		}

		// Transform ray into local gameObject space.
		float4x4 gameObjectGlobal = gameObjects[hit.second]->transform->GetGlobalMatrix();
		gameObjectGlobal.InverseAccurate();
		LineSegment temporalRay = ray;
		temporalRay.Transform(gameObjectGlobal);

		// Check if the ray intersects any of the triangles
		for (int i = 0; i < triangles.size(); i++)
		{
			if (temporalRay.Intersects(triangles[i], nullptr, nullptr))
			{
				return gameObjects[hit.second]; // If a ray intersects, return hit gameObject.
			}
		}
	}


	return nullptr;
}
