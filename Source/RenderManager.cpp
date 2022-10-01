#include "Headers.h"
#include "RenderManager.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

RenderManager::RenderManager()
{
    basicShader = new Shader("../Source/shaders/basic.vertex.shader", "../Source/shaders/basic.fragment.shader");

    Mesh cube;
    cube.InitAsCube({ 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f });

    SetMeshInformation(cube);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Mesh cube2;
            cube2.InitAsMeshInformation({ i + 1.0f,1.0f, j + 1.0f }, { 1.0f,1.0f,1.0f });
            AddMesh(cube2);
        }
    }
}

RenderManager::~RenderManager()
{
    //for (int i = 0; i < meshes.size(); i++)
    //{
    //    meshes[i].CleanUp();
    //}
    RELEASE(basicShader);
}

void RenderManager::SetMeshInformation(Mesh& mesh)
{
    // Set this RenderManager Mesh information.
    this->totalVertices.insert(totalVertices.begin(), mesh._vertices->begin(), mesh._vertices->end());
    this->totalIndices.insert(totalIndices.begin(), mesh._indices->begin(), mesh._indices->end());

    CreateBuffers();
    
    Mesh firstMesh;
    firstMesh.InitAsMeshInformation(mesh.position, mesh.scale);

    AddMesh(firstMesh); // Adds a copy of the original mesh into the mesh list.
    
    mesh.CleanUp(); // Destroy the original vertex and index data (now it is stored inside this render manager).
}

void RenderManager::Draw()
{
    if (meshes.empty()) return;

    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Update();
        modelMatrices.push_back(meshes[i].modelMatrix); // Insert updated matrices
    }

    // Draw using Dynamic Geometry
    glBindVertexArray(VAO);

    // Update Model matrices
    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &modelMatrices.front(), modelMatrices.size() * sizeof(mat4x4));
    glUnmapBuffer(GL_ARRAY_BUFFER);


    // Update View and Projection matrices
    basicShader->Bind();
    basicShader->SetMatFloat4v("view", Application::Instance()->camera->GetViewMatrix());
    basicShader->SetMatFloat4v("projection", Application::Instance()->renderer3D->GetProjectionMatrix());

    // Draw
    glDrawElementsInstanced(GL_TRIANGLES, totalIndices.size(), GL_UNSIGNED_INT, 0, debugIntSlider);
    
    glBindVertexArray(0);
    
    // Reset model matrices.
    modelMatrices.clear();
}

void RenderManager::TestOnEditor()
{
    ImGui::Begin("Testing geometry");
    
    ImGui::DragFloat3("Position: ", &meshes[0].position.x, 0.1f);
    ImGui::DragInt("Instances: ", &debugIntSlider, 1, modelMatrices.size());

    ImGui::End();
}

void RenderManager::AddMesh(Mesh& mesh)
{
    meshes.push_back(mesh);
}

void RenderManager::CreateBuffers()
{
    // Create Vertex Array Object
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    glCreateBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * totalVertices.size(), &totalVertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);


    // Create Index Buffer Object
    glCreateBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * totalIndices.size(), &totalIndices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Create Model Matrix buffer object
    glCreateBuffers(1, &MBO);

    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4x4) * 1000000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);

    // You can't pass an entire matrix, so we go row by row.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(mat4x4), (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4x4), (void*)sizeof(float4));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4x4), (void*)(sizeof(float4) * 2));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4x4), (void*)(sizeof(float4) * 3));

    // Set instancing interval
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);
}
