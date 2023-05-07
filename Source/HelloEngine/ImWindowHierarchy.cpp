#include "Headers.h"
#include "ImWindowHierarchy.h"
#include "GameObject.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ModuleRenderer3D.h"
#include "RenderManager.h"
#include "ModuleResourceManager.h"
#include "ModuleCommand.h"
#include "MeshImporter.h"
#include "ComponentUIInput.h"

ImWindowHierarchy::ImWindowHierarchy()
{
	windowName = "Hierarchy";

    isEnabled = true;

    _app = Application::Instance();

    _gameObjectsReference = &ModuleLayers::gameObjects;

    _popUpOpen = false;

    _hasSelectedAGameObject = false;

    LayerEditor::draggedGameObject = nullptr;

    _base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
}

ImWindowHierarchy::~ImWindowHierarchy()
{
}

void ImWindowHierarchy::Update()
{
	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoResize))
    {
        if (ImGui::IsWindowFocused())
            LayerEditor::detectInput = true;

        _hasSelectedAGameObject = false;
        
        DrawGameObjectChildren(ModuleLayers::rootGameObject);

        ImGui::BeginChild("DropArea");
        {
            // WindowHierachy PopUp
            if(ImGui::BeginPopupContextWindow("WinodwHierachyPopUp", ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_MouseButtonDefault_))
            {
                LayerEditor::S_SetSelectGameObject(nullptr);

                DrawOptions();

                ImGui::EndPopup();
            }            
        }
        ImGui::EndChild();

        // Create Droped mesh
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model"))
            {
                //Drop asset from Asset window to scene window
                const uint* drop = (uint*)payload->Data;

                ResourceModel* resource = (ResourceModel*)ModuleResourceManager::S_LoadResource(*drop);

                MeshImporter::LoadModelIntoScene(resource);

                std::string popUpmessage = "Loaded Mesh: ";

                LayerEditor::S_AddPopUpMessage(popUpmessage);

            }
            else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Mesh"))
            {
                const uint* drop = (uint*)payload->Data;

                ResourceMesh* resource = (ResourceMesh*)ModuleResourceManager::resources[*drop];

                GameObject* newGameObject = new GameObject(ModuleLayers::rootGameObject, resource->debugName);
                
                MeshRenderComponent* meshRender = newGameObject->AddComponent<MeshRenderComponent>();
                
                meshRender->CreateMesh(*drop);

                std::string popUpmessage = "Loaded Mesh: " + resource->debugName;

                LayerEditor::S_AddPopUpMessage(popUpmessage);

            }
            else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Prefab"))
            {
                const std::string* drop = (std::string*)payload->Data;

                ModuleResourceManager::S_DeserializeFromPrefab(*drop, ModuleLayers::rootGameObject);

                std::string popUpmessage = "Prefab Loaded: " + *drop;

                LayerEditor::S_AddPopUpMessage(popUpmessage);

            }
            ImGui::EndDragDropTarget();
        }
    }
	ImGui::End();
}

void ImWindowHierarchy::DrawGameObjectChildren(GameObject* gameObject, bool onlyChildren)
{
    if (!onlyChildren) 
        ProcessGameObject(gameObject, 0);
    else
        for (int i = 0; i < gameObject->_children.size(); i++)
        {
            ProcessGameObject(gameObject->_children[i], i);
        }
            
}

void ImWindowHierarchy::ProcessGameObject(GameObject* gameObject, int iteration)
{
    if (!gameObject || gameObject->_isPendingToDelete) 
        return;

    ImGuiTreeNodeFlags node_flags = _base_flags;
    if (gameObject->_prefabUID != 0) node_flags |= ImGuiTreeNodeFlags_Framed;

    GameObject* temp = LayerEditor::selectedGameObject;

    if (gameObject == temp)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    bool node_open;
    bool isLeaf = false;
    ImGui::AlignTextToFramePadding();

    if (gameObject->_children.empty())
    {
        isLeaf = true;
        node_flags |= ImGuiTreeNodeFlags_Leaf;
        if (gameObject->isSelected)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 1,0,0,1 });
            ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
            ImGui::PopStyleColor();
        }
        else
        {
            ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
        }
        node_open = false;

    }
    else
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);

    // Drag & drop
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", &gameObject->_ID, sizeof(uint));
        LayerEditor::draggedGameObject = gameObject;
        ImGui::Text("Change game object parent");
    
        ImGui::EndDragDropSource();
    }
    //if (ImGui::BeginDragDropSource())
    //{
    //    ImGui::SetDragDropPayload("APIGameObject", &gameObject->_ID, sizeof(uint));

    //    ImGui::EndDragDropSource();
    //}
    if (ImGui::BeginDragDropTarget())
    {   
        if (ImGui::AcceptDragDropPayload("GameObject"))
        {
            ModuleCommand::S_SetParentGameObject(LayerEditor::draggedGameObject, gameObject);
            LayerEditor::draggedGameObject = nullptr;
        }
        else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Prefab"))
        {
            const std::string* drop = (std::string*)payload->Data;

            ModuleResourceManager::S_DeserializeFromPrefab(*drop, gameObject);

            std::string popUpmessage = "Prefab Loaded: " + *drop;

            LayerEditor::S_AddPopUpMessage(popUpmessage);

        }
        ImGui::EndDragDropTarget();
    }

    // Select gameObejct
    if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left) || 
        ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
        if (ImGui::IsItemHovered() && gameObject->_parent != nullptr)
        {
            LayerEditor::S_SetSelectGameObject(gameObject);
            _hasSelectedAGameObject = true;
        }

    // Draw PopUps
    if(gameObject == LayerEditor::selectedGameObject)
    {
        if (gameObject->_parent)
        {
            if (ImGui::BeginPopupContextItem("gameObjectPopUps"))
            {
                DrawOptions();

                ImGui::EndPopup();
            }
        }
        else
            _hasSelectedAGameObject = false;
    }
 
    ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 5);

    if (ImGui::SmallButton(gameObject->IsActive() ? "X" : " "))
        gameObject->SetActive(!gameObject->IsActive());

    if (node_open)
    {
        if (!gameObject->_children.empty()) 
            DrawGameObjectChildren(gameObject, true); 
        ImGui::TreePop();
    }
    if (isLeaf)
        ImGui::TreePop();
}

void ImWindowHierarchy::DrawOptions()
{
    int selectedShape = 0;
    int selectedUI = 0;
    std::string shapeNames[5] = { "Cube", "Sphere", "Cylinder", "Plane", "Canvas"};
    std::string UINames[6] = { "Button", "CheckBox", "Slider", "Image", "Text", "VideoPlayer"};

    if (LayerEditor::selectedGameObject != nullptr)
    {
        if (ImGui::Selectable("Delete GameObject##"))
        {
             ModuleCommand::S_DeleteGameObject(LayerEditor::selectedGameObject);
        }
    }

    if (LayerEditor::selectedGameObject != nullptr)
    {
        if (LayerEditor::selectedGameObject->GetTag() == "UI" || LayerEditor::selectedGameObject->GetTag() == "UIsliderButton" || LayerEditor::selectedGameObject->GetTag() == "UIsliderBar")
        {
            if (ImGui::Selectable("Create Panel"))
            {
                GameObject* parent = LayerEditor::selectedGameObject ? LayerEditor::selectedGameObject : ModuleLayers::rootGameObject;
                GameObject* panel = new GameObject(parent, "Panel", "UI");
                panel->AddComponent<ComponentUIInput>();
            }
            for (int i = 0; i < 6; i++)
            {
                if (ImGui::Selectable(UINames[i].c_str()))
                {
                    selectedUI = i;
                    _app->renderer3D->renderManager.CreateUI(LayerEditor::selectedGameObject, (UIType)i);
                }
            }
        }
        else
        {
            if (ImGui::Selectable("Create empty GameObject"))
            {
                GameObject* parent = LayerEditor::selectedGameObject ? LayerEditor::selectedGameObject : ModuleLayers::rootGameObject;
                GameObject* newGameObject = new GameObject(parent, "Empty");
            }

            for (int i = 0; i < 5; i++)
            {
                if (ImGui::Selectable(shapeNames[i].c_str()))
                {
                    selectedShape = i;
                    _app->renderer3D->renderManager.CreatePrimitive(LayerEditor::selectedGameObject, (PrimitiveType)i);
                }
            }
        }
    }
    else
    {    
        if (ImGui::Selectable("Create empty GameObject"))
        {
            GameObject* parent = LayerEditor::selectedGameObject ? LayerEditor::selectedGameObject : ModuleLayers::rootGameObject;
            GameObject* newGameObject = new GameObject(parent, "Empty");
        }

        for (int i = 0; i < 5; i++)
        {
            if (ImGui::Selectable(shapeNames[i].c_str()))
            {
                selectedShape = i;
                _app->renderer3D->renderManager.CreatePrimitive(LayerEditor::selectedGameObject, (PrimitiveType)i);
            }
        }
    }
}