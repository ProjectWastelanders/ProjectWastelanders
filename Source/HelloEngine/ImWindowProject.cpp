#include "Headers.h"
#include "ImWindowProject.h"
#include "ModuleFiles.h"
#include "FileTree.hpp"
#include "ModuleResourceManager.h"

#include "ModuleWindow.h"
#include "ModuleLayers.h"
#include "TextureImporter.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"

#include "BaseShader.h"

ImWindowProject::ImWindowProject()
{
	windowName = "Project";

	isEnabled = true;

	_app = Application::Instance();

	_window = ModuleWindow::window;

	ModuleResourceManager::S_GetFileTree(_fileTree);

	// Init delete object;
	_deleteFile = nullptr;

	// Init rootNode & currentNode
	UpdateFileNodes();

	// Add onDropListener event
	ModuleInput::S_AddOnDropListener(std::bind(&ImWindowProject::OnDrop, this, std::placeholders::_1));

	// Load Icons 
	{
		char* buffer = nullptr;
		int size = ModuleFiles::S_Load("Resources/Editor/Images/files.dds", &buffer);
		_fileImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);

		buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/folder.dds", &buffer);
		_folderImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);

		buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/grid.dds", &buffer);
		_meshImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer);

		buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/modelOpen.dds", &buffer);
		_modelImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer)

			buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/scene.dds", &buffer);
		_sceneImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer)

			buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/image.dds", &buffer);
		_textureImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer)

			buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/h.dds", &buffer);
		_hImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer)

			buffer = nullptr;
		size = ModuleFiles::S_Load("Resources/Editor/Images/cpp.dds", &buffer);
		_cppImageID = TextureImporter::LoadEditorDDS(buffer, size);
		RELEASE(buffer)
	}
}

ImWindowProject::~ImWindowProject()
{
	ModuleInput::S_ClearOnDropListener();
}

void ImWindowProject::Update()
{
	CheckWindowFocus();

	if (_showDeleteMessage)
		DrawDeleteMessage();

	if (ImGui::Begin(windowName.c_str(), &isEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
	{
		// Options, Filter
		//if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
		//ImGui::SameLine();
		//filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
		//ImGui::SameLine();

		// For change directory
		Directory* newDir = nullptr;

		// Resize children width
		// Warning, is static!!!
		static float widthLeft = 200; // Init Size child 1
		static float widthRight = 1200; // Init Size child 2

		// Adjust window size
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::DrawSplitter(0, 10, &widthLeft, &widthRight, 100, 200);

		widthRight = (windowSize.x - widthLeft - 20);

		// Left window
		if (widthLeft > 0)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1));

			if (ImGui::BeginChild("ChildL", ImVec2(widthLeft, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
				DrawTreeNodePanelLeft(newDir, _rootNode, false);
			ImGui::EndChild();

			ImGui::PopStyleColor(1);
		}

		ImGui::SameLine();

		// Right window
		if (widthRight > 0)
		{
			if (ImGui::BeginChild("ChildR", ImVec2(widthRight, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
				DrawTreeNodePanelRight(newDir);
			ImGui::EndChild();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
			{
				//Drop asset from Asset window to scene window
				const uint* drop = (uint*)payload->Data;

				ModuleResourceManager::S_SerializeToPrefab(ModuleLayers::gameObjects[*drop], _fileTree->_currentDir->path);
			}
			ImGui::EndDragDropTarget();
		}

		// Change current directory
		if (newDir)
			_fileTree->_currentDir = newDir;
	}
	ImGui::End();

	if (_openCreateFolderPanel)
		PanelCreateFolder();

	else if (_openCreateScriptPanel)
		PanelCreateScript();

	if (_reimportRequest)
	{
		ImGui::OpenPopup("Reimport resources");

		if (ImGui::BeginPopupModal("Reimport resources", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("The resources is being reimported, please wait.");
			ImGui::EndPopup();
		}

		if (_reimportCounter <= 0)
		{
			for (auto item : _selectedFiles)
			{
				if (item->GetFileType() == FileType::File)
				{
					File* tempFile = (File*)item;

					tempFile->Reimport();
				}
				else if (item->GetFileType() == FileType::Directory)
				{
					Directory* tempFile = (Directory*)item;

					RefreshAssetsPerDir(tempFile);
				}

				item->SetSelected(false);
			}
			
			_selectedFiles.clear();
			//if (_reimportFile)
			//{
			//	_reimportFile->Reimport();

			//	_reimportFile = nullptr;
			//}
			//else if (_reimportDir)
			//{
			//	RefreshAssetsPerDir(_reimportDir);

			//	_reimportDir = nullptr;
			//}

			_reimportRequest = false;
		}
		else
		{
			_reimportCounter--;
		}
	}

	// If have any file to delete, delete this
	if (_deleteFile && _deleteFileAccepted)
	{
		ModuleFiles::S_Delete(_deleteFile->path);

		if (_deleteFile->metaPath != "none")
			ModuleResourceManager::S_DeleteMetaFile(_deleteFile->metaPath);

		_deleteFile = nullptr;
		_deleteFileAccepted = false;

		UpdateFileNodes();
	}
    else if (_openCreateShaderPanel)
        PanelCreateShader();

    else if (_openCreateMaterialPanel)
        PanelCreateMaterial();
	// If have any folder to delete, delete this
	if (_deleteDir && _deleteFileAccepted)
	{
		ModuleFiles::S_Delete(_deleteDir->path);

		UpdateFileNodes();

		_deleteDir = nullptr;
		_deleteFileAccepted = false;
	}
}

void ImWindowProject::RefreshAssetsPerDir(Directory* dir)
{
	for (size_t i = 0; i < dir->files.size(); i++)
	{
		if (dir->files[i].metaFile.type == ResourceType::MODEL
			|| dir->files[i].metaFile.type == ResourceType::TEXTURE)
			dir->files[i].Reimport();
	}

	for (size_t i = 0; i < dir->directories.size(); i++)
	{
		RefreshAssetsPerDir(dir->directories[i]);
	}
}

void ImWindowProject::DrawTreeNodePanelLeft(Directory*& newDir, Directory* node, const bool drawFiles) 
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_OpenOnArrow;

	if (node == _fileTree->_currentDir)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (node->directories.empty())
		node_flags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(node->name.c_str(), node_flags))
	{
		// Slect node
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			ClearSelectedFiles();
			newDir = node;
		}
			
		// Recursive functions
		for (int i = 0; i < node->directories.size(); i++)
			DrawTreeNodePanelLeft(newDir, node->directories[i], drawFiles);

		// Draw Files
		if (drawFiles)
		{
			for (int i = 0; i < node->files.size(); i++)
			{
				ImGui::Text(node->files[i].name.c_str());
			}
		}

		ImGui::TreePop();
	}
}

void ImWindowProject::DrawTreeNodePanelRight(Directory*& newDir)
{
	// Return buttons
	{
		Directory* parent1 = nullptr;
		Directory* parent2 = nullptr;

		if (_fileTree->_currentDir->parent)
			parent1 = _fileTree->_currentDir->parent;

		if (parent1 && parent1->parent)
			parent2 = parent1->parent;

		if (parent2)
		{
			std::string pName = parent2->name + " > ##Return";

			if (ImGui::Button(pName.c_str()))
			{
				newDir = parent2;

				ClearSelectedFiles();
			}
		}
		if (parent1)
		{
			ImGui::SameLine();
			std::string pName = parent1->name + " > ##Return";

			if (ImGui::Button(pName.c_str()))
			{
				newDir = parent1;

				ClearSelectedFiles();
			}
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.4, 0.4, 255));

		if (ImGui::Button(_fileTree->_currentDir->name.c_str()))
		{
			// Do nothing here, just fixed space
		}

		ImGui::PopStyleColor(1);
	}

	ImGui::Separator();

	// Calculate num of colomns we can have
	int numOfColumns = (ImGui::GetContentRegionAvail().x / _itemWidth) - 1;

	if (numOfColumns == 0)
		numOfColumns++;

	ImGui::Columns(numOfColumns, "files columns", false);

	// Folders
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.3, 0.3, 0));
	for (int i = 0; i < _fileTree->_currentDir->directories.size(); i++)
	{
		Directory* currentDir = _fileTree->_currentDir->directories[i];

		std::string directoryID = std::to_string(i).c_str() + currentDir->name;

		// Left click
		if (ImGui::ImageButton(directoryID.c_str(), (ImTextureID)_folderImageID, ImVec2(_itemWidth, _itemHeight)))
		{
			if (!currentDir->IsSelected())
			{
				if (ModuleInput::S_GetKey(SDL_SCANCODE_LCTRL) != KEY_STATE::KEY_REPEAT)
				{
					ClearSelectedFiles();
				}

				currentDir->SetSelected(true);

				_selectedFiles.push_back(currentDir);
			}
			else
			{
				newDir = currentDir;
				ClearSelectedFiles();
			}
		}

		// Right click
		if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
		{
			if (!currentDir->IsSelected())
			{
				ClearSelectedFiles();

				currentDir->SetSelected(true);

				_selectedFiles.push_back(currentDir);
			}

			if (ImGui::Button("Reimport##Folder"))
			{
				_reimportRequest = true;
				_reimportCounter = 100;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Delete##Folder"))
			{
				_deleteDir = currentDir;
				_showDeleteMessage = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(currentDir->name.c_str());

		// Show File name
		if (currentDir->IsSelected())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.6f, 1.0f));
			ImGui::TextWrapped(currentDir->name.c_str());
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::TextWrapped(currentDir->name.c_str());
		}

		ImGui::NextColumn();
	}
	ImGui::PopStyleColor(1);

	// Files
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.3, 0.3, 0));
	for (int i = 0; i < _fileTree->_currentDir->files.size(); i++)
	{
		File* currentFile = &_fileTree->_currentDir->files[i];

		// Get file icon ID
		uint icon = 0;
		switch (ModuleFiles::S_GetResourceType(currentFile->path))
		{
		case ResourceType::MODEL:
			icon = _modelImageID;
			break;
		case ResourceType::SCENE:
			icon = _sceneImageID;
			break;
		case ResourceType::TEXTURE:
			icon = _textureImageID;
			break;
		case ResourceType::HSCRIPT:
			icon = _hImageID;
			break;
		case ResourceType::CPPSCRIPT:
			icon = _cppImageID;
			break;
		default:
			icon = _fileImageID;
			break;
		}

		std::string fileID = std::to_string(i).c_str() + currentFile->name;

		// Left click
		if (ImGui::ImageButton(fileID.c_str(), (ImTextureID)icon, ImVec2(_itemWidth, _itemHeight)))
		{
			currentFile->pressed = !currentFile->pressed;
			currentFile->SetSelected(true);
			if (ModuleInput::S_GetKey(SDL_SCANCODE_LCTRL) != KEY_STATE::KEY_REPEAT)
			{
				ClearSelectedFiles();
			}
			_selectedFiles.push_back(currentFile);
		}

		// Drag file
		ResourceType type = ModuleFiles::S_GetResourceType(currentFile->name);

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			switch (type)
			{
			case ResourceType::TEXTURE:
				// Find resource path
				_dragUID = _fileTree->_currentDir->files[i].metaFile.UID;

				// Set payload to carry the index of our item (could be anything)
				ImGui::SetDragDropPayload("Texture", &_dragUID, sizeof(uint));
				break;
			case ResourceType::MODEL:
				_dragUID = _fileTree->_currentDir->files[i].metaFile.UID;

				ImGui::SetDragDropPayload("Model", &_dragUID, sizeof(uint));
				break;
			case ResourceType::SCENE:
				ImGui::SetDragDropPayload("Scene", &_fileTree->_currentDir->files[i].path, sizeof(std::string));
				break;
			case ResourceType::HSCRIPT:
			case ResourceType::CPPSCRIPT:
				_dragUID = _fileTree->_currentDir->files[i].metaFile.UID;
				ImGui::SetDragDropPayload("Script", &_dragUID, sizeof(uint));
				break;
			case ResourceType::ANIMATION:
				_dragUID = _fileTree->_currentDir->files[i].metaFile.UID;
				ImGui::SetDragDropPayload("Animation", &_dragUID, sizeof(uint));
				break;
			case ResourceType::PREFAB:
				ImGui::SetDragDropPayload("Prefab", &_fileTree->_currentDir->files[i].path, sizeof(std::string));
				break;
			case ResourceType::SHADER:
                _dragUID = _fileTree->_currentDir->files[i].metaFile.UID;
                ImGui::SetDragDropPayload("Shader", &_dragUID, sizeof(uint));
                break;
            case ResourceType::MATERIAL:
                _dragUID = _fileTree->_currentDir->files[i].metaFile.UID;
                ImGui::SetDragDropPayload("Material", &_dragUID, sizeof(uint));
                break;
			case ResourceType::VIDEO:
				_dragUID = _fileTree->_currentDir->files[i].metaFile.UID;
				ImGui::SetDragDropPayload("Video", &_dragUID, sizeof(uint));
				break;
			}
			ImGui::EndDragDropSource();
		}

		// Right click
		if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
		{
			if (!currentFile->IsSelected())
			{
				ClearSelectedFiles();

				currentFile->SetSelected(true);

				_selectedFiles.push_back(currentFile);
			}

			if (ImGui::Button("Reimport##File"))
			{
				_reimportRequest = true;
				_reimportCounter = 100;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Delete##File"))
			{
				_deleteFile = &_fileTree->_currentDir->files[i];
				_showDeleteMessage = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

  		static bool doubleClick = false;

		// Shwo file name when mouse is hovered
		if (ImGui::IsItemHovered())
		{
			 if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                doubleClick = true;
            }
			ImGui::SetTooltip(currentFile->name.c_str());
		}

		//Double click
        if (doubleClick)
        {
            type = ModuleFiles::S_GetResourceType(_fileTree->_currentDir->files[i].name);
            switch (type)
            {
            case ResourceType::TEXTURE:
                break;
            case ResourceType::MODEL:
                break;
            case ResourceType::SCENE:
                break;
            case ResourceType::HSCRIPT:
            case ResourceType::CPPSCRIPT:
                break;
            case ResourceType::ANIMATION:
                break;
            case ResourceType::PREFAB:
                break;
            case ResourceType::SHADER:
                LayerEditor::S_OpenShader(_fileTree->_currentDir->files[i].metaFile.UID);
                break;
            case ResourceType::MATERIAL:
                break;
            }
            doubleClick = false;
        }

		// Show file name
		if (currentFile->IsSelected())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.6f, 1.0f));
			ImGui::TextWrapped(currentFile->name.c_str());
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::TextWrapped(currentFile->name.c_str());
		}

		// Draw Mesh files
		if (currentFile->metaFile.type == ResourceType::MODEL && currentFile->pressed)
		{
			ResourceModel* model = (ResourceModel*)ModuleResourceManager::resources[_fileTree->_currentDir->files[i].metaFile.UID];

			if (_fileTree->_currentDir->files[i].metaFile.UID == 2417919063)
			{
				std::cout << "a";
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.3, 0.3, 1));
			for (int j = 0; j < model->modelMeshes.size(); j++)
			{
				ImGui::NextColumn();
				ImGui::ImageButton(std::to_string(model->modelMeshes[j]->UID).c_str(), (ImTextureID)_meshImageID, ImVec2(_itemWidth, _itemHeight));

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Find resource path
					_dragUID = model->modelMeshes[j]->UID;

					// Set payload to carry the index of our item (could be anything)
					ImGui::SetDragDropPayload("Mesh", &_dragUID, sizeof(uint));

					ImGui::EndDragDropSource();
				}
				ImGui::TextWrapped(model->modelMeshes[j]->debugName.c_str());
			}
			ImGui::PopStyleColor(1);
		}

		ImGui::NextColumn();
	}
	ImGui::PopStyleColor(1);

	// Right Click in the panel
	if (ImGui::BeginPopupContextWindow("WinodwProjectPopUp", ImGuiPopupFlags_NoOpenOverExistingPopup
		| ImGuiPopupFlags_MouseButtonDefault_))
	{
		ClearSelectedFiles();

		if (ImGui::Selectable("Show in Explorer"))
			ModuleFiles::S_OpenFolder(_fileTree->_currentDir->path);

		if (ImGui::Selectable("Create Folder"))
			_openCreateFolderPanel = true;

		if (ImGui::Selectable("Create Script"))
			_openCreateScriptPanel = true;

		if (ImGui::Selectable("Create Shader"))
        {
            _openCreateShaderPanel = true;
            _temporalName = "newShader";
        }
           
        if (ImGui::Selectable("Create Material"))
        {
            _openCreateMaterialPanel = true;
            _temporalName = "newMaterial";
        }

		ImGui::EndPopup();
	}
}

void ImWindowProject::OnDrop(const std::string filePath)
{
	std::string pathNormalized = ModuleFiles::S_NormalizePath(filePath);

	// Use this for global path files
	ModuleFiles::S_ExternalCopy(pathNormalized, _fileTree->_currentDir->path);

	std::string relativePath = _fileTree->_currentDir->path + ModuleFiles::S_GetFileName(pathNormalized);

	// File case
	File file = File(relativePath, ModuleFiles::S_GetFileName(pathNormalized), _fileTree->_currentDir);//_fileTree->_currentDir->files.emplace_back(relativePath, ModuleFiles::S_GetFileName(pathNormalized), _fileTree->_currentDir);

	_fileTree->_currentDir->files.push_back(file);
}

void ImWindowProject::UpdateFileNodes()
{
	ClearSelectedFiles();

	_selectedFiles.clear();

	ModuleResourceManager::S_UpdateFileTree();

	if (ModuleResourceManager::S_GetFileTree(_fileTree))
		_fileTree->GetRootDir(_rootNode);
}

void ImWindowProject::RefreshAssets()
{
	Directory* _reimportDir = nullptr;

	_fileTree->GetRootDir(_reimportDir);

	_reimportRequest = true;

	_reimportCounter = 100;

	ClearSelectedFiles();

	_selectedFiles.push_back(_reimportDir);
}

void ImWindowProject::CheckWindowFocus()
{
	Uint32 flags = SDL_GetWindowFlags(_window);

	// When Global Windows has selected -> just the instante
	if ((flags & SDL_WINDOW_INPUT_FOCUS) != 0)
	{
		if (!_isWindowFocus)
		{
			_isWindowFocus = true;

			UpdateFileNodes();
		}
	}
	// When Global Windows has deselected -> just the instante
	else if (_isWindowFocus)
		_isWindowFocus = false;
}

void ImWindowProject::PanelCreateFolder()
{
	ImGui::OpenPopup("Insert Name##Folder");
	if (ImGui::BeginPopupModal("Insert Name##Folder", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::InputText("##inputTextFolderName", &_temporalName);

		if (ImGui::Button("Accept"))
		{
			if (ModuleFiles::S_MakeDir(_fileTree->_currentDir->path + _temporalName))
			{
				_fileTree->_currentDir->directories.push_back(
					new Directory(
						_fileTree->_currentDir->path + _temporalName + "/",
						_temporalName,
						_fileTree->_currentDir)
				);
			}

			_temporalName = "default";

			_openCreateFolderPanel = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			_temporalName = "default";

			_openCreateFolderPanel = false;
		}

		ImGui::EndPopup();
	}
}

void ImWindowProject::PanelCreateScript()
{
	ImGui::OpenPopup("Insert Name##Script");
	if (ImGui::BeginPopupModal("Insert Name##Script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::InputText("##inputTextScriptName", &_temporalName);

		if (ImGui::Button("Accept"))
		{
			if (ModuleFiles::S_CreateScriptFile(_temporalName, _fileTree->_currentDir->path.c_str()))
			{
				_fileTree->_currentDir->files.emplace_back(_fileTree->_currentDir->path + _temporalName + ".h", _temporalName + ".h", _fileTree->_currentDir);
				_fileTree->_currentDir->files.emplace_back(_fileTree->_currentDir->path + _temporalName + ".cpp", _temporalName + ".cpp", _fileTree->_currentDir);
			}
			else
				LayerEditor::S_AddPopUpMessage("The script with this name is already exist");

			_temporalName = "default";

			_openCreateScriptPanel = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			_temporalName = "default";

			_openCreateScriptPanel = false;
		}


		ImGui::EndPopup();
	}
}

void ImWindowProject::PanelCreateShader()
{
    ImGui::OpenPopup("Insert Name##Shader");

    if (ImGui::BeginPopupModal("Insert Name##Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Name: "); ImGui::SameLine();
        ImGui::InputText("##inputShaderName", &_temporalName);
        ImGui::SameLine();
        ImGui::Text(".shader");

        if (ImGui::Button("Accept"))
        {
            _temporalName.append(".shader");

			uint UUID = HelloUUID::GenerateGUID(_fileTree->_currentDir->path + _temporalName);

            std::string resourcePath = "Resources/Shaders/" + std::to_string(UUID) + ".shader";
            std::string assetPath = _fileTree->_currentDir->path + _temporalName;

			if (ModuleFiles::S_Exists(resourcePath)) 
			{
				LayerEditor::S_AddPopUpMessage("A shader with that name already exists.");
				return;
			}

            int size = BaseShader::newShaderTextFileUnlit.length();
            char* cstr = BaseShader::newShaderTextFileUnlit.data();

            //Save Shader to resources
            ModuleFiles::S_Save(resourcePath, cstr, size, false);

            //Save duplicated file into assets
            ModuleFiles::S_Save(assetPath, cstr, size, false);

            //Create Metadata
            ModuleFiles::S_CreateMetaData(assetPath, resourcePath, UUID);

            _temporalName = "default";

            _openCreateShaderPanel = false;
            delete[] cstr;

			UpdateFileNodes();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            _temporalName = "default";

            _openCreateShaderPanel = false;
        }

        ImGui::EndPopup();
    }
}

void ImWindowProject::PanelCreateMaterial()
{
    ImGui::OpenPopup("Insert Name##Material");

    if (ImGui::BeginPopupModal("Insert Name##Material", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Name: "); ImGui::SameLine();
        ImGui::InputText("##inputMaterialName", &_temporalName);
        ImGui::SameLine();
        ImGui::Text(".material");

        if (ImGui::Button("Accept"))
        {
            _temporalName.append(".material");

			uint UUID = HelloUUID::GenerateGUID(_fileTree->_currentDir->path + _temporalName);

            std::string resourcePath = "Resources/Materials/" + std::to_string(UUID) + ".material";
            std::string assetPath = _fileTree->_currentDir->path + _temporalName;

			if (ModuleFiles::S_Exists(resourcePath))
			{
				LayerEditor::S_AddPopUpMessage("A material with that name already exists.");
				return;
			}

			json emptyMaterial;

			emptyMaterial["Shader resource UID"] = 0;

			json uni;

			uni["Name"] = "Null";
			uni["Type"] = 0;
			uni["String Type"] = "Null";
			uni["Index"] = 0;
			uni["Read"] = 0;
			uni["Size"] = 0;

			emptyMaterial["Uniforms"] = uni;

			std::string buffer = emptyMaterial.dump(4);

            //Resources
            ModuleFiles::S_Save(resourcePath, &buffer[0], buffer.size(), false);

            //Assets
            ModuleFiles::S_Save(assetPath, &buffer[0], buffer.size(), false);

            //Create Metadata
            ModuleFiles::S_CreateMetaData(assetPath, resourcePath, UUID);

            _temporalName = "default";

            _openCreateMaterialPanel = false;
			UpdateFileNodes();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            _temporalName = "default";

            _openCreateMaterialPanel = false;
        }

        ImGui::EndPopup();
    }
}

void ImWindowProject::DrawDeleteMessage()
{
	ImGui::OpenPopup("Delete file");
	if (ImGui::BeginPopupModal("Delete file", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("WARNING: This action cannot be undone!");
		ImGui::Text("Are you sure you want to delete this file? Any reference to it on the current scene will be destroyed.");

		if (ImGui::Button("Close"))
		{
			_deleteFile = nullptr;
			_showDeleteMessage = false;
		}
		ImGui::SameLine();

		if (ImGui::Button("Delete"))
		{
			_deleteFileAccepted = true;
			_showDeleteMessage = false;
		}
		ImGui::EndPopup();
	}
}

void ImWindowProject::ClearSelectedFiles()
{
	for (auto item : _selectedFiles)
	{
		item->SetSelected(false);
	}

	_selectedFiles.clear();
}
