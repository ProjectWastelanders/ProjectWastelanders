#include "Headers.h"
#include "InstanceRenderer.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "MeshRenderComponent.h"

#include "RenderManager.h"

InstanceRenderer::InstanceRenderer()
{
    instancedShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/instanced.shader", 102, "Instanced");
    perMeshShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/basic.shader", 103, "Basic");
    mesh2DShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/instanced2D.shader", 104, "Instanced 2D");
    particleShader = ModuleResourceManager::S_CreateResourceShader("Resources/shaders/Particles.shader", 114, "Particles");
}

InstanceRenderer::~InstanceRenderer()
{
    instancedShader->Dereference();
    instancedShader = nullptr;
    perMeshShader->Dereference();
    perMeshShader = nullptr;
    mesh2DShader->Dereference();
    mesh2DShader = nullptr;

    if (VAO == 0)
        return;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &MBO);
    glDeleteBuffers(1, &TBO);

    glDeleteVertexArrays(1, &BasicVAO);
    glDeleteBuffers(1, &BasicVBO);
    glDeleteBuffers(1, &BasicIBO);
}

void InstanceRenderer::SetMeshInformation(ResourceMesh* resMesh, ResourceMaterial* resMat)
{
    if (initialized) 
        LOG("Tried to call RenderManager::SetMeshInformation more than once in a single Render Manager instance.");
    
    if (resMesh == nullptr)
        return;

    // Set this RenderManager Mesh information.
    totalVertices = &resMesh->meshInfo.vertices;
    totalIndices = &resMesh->meshInfo.indices;
    this->resource = resMesh;
    this->resMat = resMat;

    CreateBuffers();
    CreateBasicBuffers();

    initialized = true;
}

void InstanceRenderer::Draw()
{
    if (!initialized) return; // This is placed here for security reasons. No RenderManager should be created without being initialized.
    if (meshes.empty())
    {
        LOG("A Render Manager is being updated without any meshes!");
        if (resource != nullptr)
            Application::Instance()->renderer3D->renderManager.DestroyRenderManager(GetRenderID());
        else
            Application::Instance()->renderer3D->renderManager.DestroyRenderManager(deletedResourceUID);
        return;
    }

    if (resMat != nullptr && resMat->material.GetShader() != nullptr)
    {
        DrawMaterial(); //Draws using the current Material
    }
    else
    {
        DrawRaw(); //Draw using no material, "Legacy" Draw
    }

}

void InstanceRenderer::DrawMaterial()
{
    for (auto& mesh : meshes)
    {
        RenderUpdateState state = mesh.second.mesh.Update();
        if (state == RenderUpdateState::NODRAW)
            continue;

        //Selected Mesh
        if (state == RenderUpdateState::SELECTED)
        {
            mesh.second.material = this->resMat;
            mesh.second.resMat = this->resMat->UID;
            Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&mesh.second);
        }

        modelMatrices.push_back(mesh.second.mesh.modelMatrix); // Insert updated matrices
    }

    if (!modelMatrices.empty())
    {
        //Update all the uniforms
        resMat->material.UpdateInstanced(Application::Instance()->camera->currentDrawingCamera->GetViewMatrix(),
            Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

        // Draw using Dynamic Geometrys
        glBindVertexArray(VAO);

        // Update Model matrices
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, &modelMatrices.front(), modelMatrices.size() * sizeof(float4x4));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        // Draw instanced
        glDrawElementsInstanced(GL_TRIANGLES, totalIndices->size(), GL_UNSIGNED_INT, 0, modelMatrices.size());
        glBindVertexArray(0);
    }

    resMat->material.UnbindAllTextures();
    // Reset model matrices.
    modelMatrices.clear();
}

void InstanceRenderer::DrawRaw()
{
    CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;

    if (is2D && currentCamera->type != CameraType::GAME)
        return;

    for (auto& mesh : meshes)
    {
        RenderUpdateState state = mesh.second.mesh.Update();
        if (state == RenderUpdateState::NODRAW)
            continue;

        if (state == RenderUpdateState::SELECTED)
        {
            Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&mesh.second.mesh);
        }

        modelMatrices.push_back(mesh.second.mesh.modelMatrix); // Insert updated matrices
        textureIDs.push_back(mesh.second.mesh.OpenGLTextureID);
        mesh.second.mesh.OpenGLTextureID = -1; // Reset this, in case the next frame our texture ID changes to -1.
    }

    if (!modelMatrices.empty())
    {
        // Update View and Projection matrices
        instancedShader->shader.Bind();

        instancedShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
        instancedShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

        // Draw using Dynamic Geometrys
        glBindVertexArray(VAO);

        // Update Model matrices
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, &modelMatrices.front(), modelMatrices.size() * sizeof(float4x4));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        // Update TextureIDs
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        void* ptr2 = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr2, &textureIDs.front(), textureIDs.size() * sizeof(float));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        for (int i = 0; i < TextureManager::bindedTextures; i++)
        {
            instancedShader->shader.SetInt(("textures[" + std::to_string(i) + "]").c_str(), i);
        }

        // Draw instanced
        glDrawElementsInstanced(GL_TRIANGLES, totalIndices->size(), GL_UNSIGNED_INT, 0, modelMatrices.size());
        glBindVertexArray(0);
    }

    // Reset model matrices.
    modelMatrices.clear();
    textureIDs.clear();
    TextureManager::UnBindTextures();
}

void InstanceRenderer::Draw2D()
{
    if (!is2D)
        return;

    CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;

    // Draw transparent objects with a draw call per mesh.
    for (auto& mesh : meshes)
    {
        float zComponent = mesh.second.mesh.modelMatrix.Transposed().TranslatePart().z;
        orderedMeshes.emplace(std::make_pair(zComponent, &mesh.second.mesh));
    }

    for (auto mesh = orderedMeshes.rbegin(); mesh != orderedMeshes.rend(); mesh++)
    {
        if (mesh->second->Update() == RenderUpdateState::DRAW)
        {
            mesh->second->Draw(Material(), false);
        }
    }
    orderedMeshes.clear();
}

uint InstanceRenderer::AddMesh()
{
    if (!initialized)
    {
        LOG("Trying to add mesh information into a RenderManager that has not been initialized yet!");
    }

    uint meshID = ++IDcounter; // We use a counter for easier debugging, but this could be an UUID.
    meshes[meshID].mesh.localAABB = resource->localAABB;
    meshes[meshID].mesh.resource = this->resource;

    // If our instance capacity is too low, reserve more memory inside the opengl buffer.
    if (instanceNum < meshes.size())
        ReallocateMoreMemory();

    return meshID;
}

void InstanceRenderer::DrawInstance(Mesh* mesh, bool useBasicShader)
{
    //
    if (useBasicShader)
    {
        if (resMat)
        {
            resMat->material.Update(Application::Instance()->camera->currentDrawingCamera->GetViewMatrix(),
                Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix(),
                &mesh->modelMatrix.v[0][0]);
        }
        else
        {
            if (mesh->textureID != -1)
            {
                glBindTexture(GL_TEXTURE_2D, mesh->textureID);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            perMeshShader->shader.Bind();
            perMeshShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
            perMeshShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());
            perMeshShader->shader.SetMatFloat4v("model", &mesh->modelMatrix.v[0][0]);
        }
    }

    glBindVertexArray(BasicVAO);

    glDrawElements(GL_TRIANGLES, totalIndices->size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void InstanceRenderer::DrawInstancedSorting()
{

    CameraObject* currentCamera = Application::Instance()->camera->currentDrawingCamera;
    float3 cameraPos = currentCamera->cameraFrustum.pos;

    std::vector<RenderEntry> _orderedMeshes;

    glDepthMask(GL_FALSE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto& mesh : meshes)
    {
        RenderUpdateState state = mesh.second.mesh.Update();
        if (state == RenderUpdateState::NODRAW)
            continue;

        if (state == RenderUpdateState::SELECTED)
        {
            Application::Instance()->renderer3D->renderManager.SetSelectedMesh(&mesh.second.mesh);
        }

        modelMatrices.push_back(mesh.second.mesh.modelMatrix); // Insert updated matrices
        textureIDs.push_back(mesh.second.mesh.OpenGLTextureID);
        mesh.second.mesh.OpenGLTextureID = -1; // Reset this, in case the next frame our texture ID changes to -1.
    }

    if (!modelMatrices.empty())
    {
        // Update View and Projection matrices
        particleShader->shader.Bind();

        particleShader->shader.SetMatFloat4v("view", Application::Instance()->camera->currentDrawingCamera->GetViewMatrix());
        particleShader->shader.SetMatFloat4v("projection", Application::Instance()->camera->currentDrawingCamera->GetProjectionMatrix());

        // Draw using Dynamic Geometrys
        glBindVertexArray(VAO);

        // Update Model matrices
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, &modelMatrices.front(), modelMatrices.size() * sizeof(float4x4));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        // Update TextureIDs
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        void* ptr2 = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr2, &textureIDs.front(), textureIDs.size() * sizeof(float));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        for (int i = 0; i < TextureManager::bindedTextures; i++)
        {
            instancedShader->shader.SetInt(("textures[" + std::to_string(i) + "]").c_str(), i);
        }

        // Draw instanced
        glDrawElementsInstanced(GL_TRIANGLES, totalIndices->size(), GL_UNSIGNED_INT, 0, modelMatrices.size());
        glBindVertexArray(0);
    }

    
    glDepthMask(GL_TRUE);

    // Reset model matrices.
    modelMatrices.clear();
    textureIDs.clear();
    TextureManager::UnBindTextures();
}

void InstanceRenderer::SetAs2D()
{
    is2D = true;
}

uint InstanceRenderer::GetRenderID()
{
    uint toReturn = 0;
    if (resource) toReturn += resource->UID;
    if (resMat) toReturn += resMat->UID;
    return toReturn;
}

void InstanceRenderer::CreateBuffers()
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * totalVertices->size(), &totalVertices->at(0), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Create Index Buffer Object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * totalIndices->size(), &totalIndices->at(0), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Create dynamic buffers based on current instanceNum.
    CreateDynamicBuffers();
}

void InstanceRenderer::CreateBasicBuffers()
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &BasicVAO);
    glBindVertexArray(BasicVAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &BasicVBO);

    glBindBuffer(GL_ARRAY_BUFFER, BasicVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * totalVertices->size(), &totalVertices->at(0), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glGenBuffers(1, &BasicIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BasicIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * totalIndices->size(), &totalIndices->at(0), GL_STATIC_DRAW);

    glBindVertexArray(0);


}

void InstanceRenderer::ReallocateMoreMemory()
{
    // Add instance num
    instanceNum += (int)(instanceNum * 0.5f);

    DestroyDynamicBuffers();
    CreateDynamicBuffers();
}

void InstanceRenderer::DestroyDynamicBuffers()
{
    glDeleteBuffers(1, &MBO);
    glDeleteBuffers(1, &TBO);
}

void InstanceRenderer::CreateDynamicBuffers()
{
    // Create Model Matrix buffer object
    glGenBuffers(1, &MBO);

    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float4x4) * instanceNum, nullptr, GL_DYNAMIC_DRAW); // TODO: This buffer size should dynamicaly change

    glBindVertexArray(VAO);

    // You can't pass an entire matrix, so we go row by row.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)sizeof(float4));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 2));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 3));

    // Set instancing interval
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // Create TextureID buffer object
    glGenBuffers(1, &TBO);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instanceNum, nullptr, GL_DYNAMIC_DRAW); // TODO: This buffer size should dynamically change

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}

