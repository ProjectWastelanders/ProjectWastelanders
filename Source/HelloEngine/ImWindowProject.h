#pragma once
#include "ImWindow.h"
#include "imgui.h"

class FileTree;
class File;
class SelectableFile;
struct Directory;

class ImWindowProject : public ImWindow
{
public:
    ImWindowProject();

    ~ImWindowProject();

    void Update() override;

    void UpdateFileNodes();

    void RefreshAssets();

private:
    void RefreshAssetsPerDir(Directory* dir);

    void DrawTreeNodePanelLeft(Directory*& newDir, Directory* node, const bool drawFiles = true);

    void DrawTreeNodePanelRight(Directory*& newDir);

    void OnDrop(const std::string filePath);

    void CheckWindowFocus();

    void PanelCreateFolder();

    void PanelCreateScript();

    void PanelCreateShader();

    void PanelCreateMaterial();

    void DrawDeleteMessage();

    void ClearSelectedFiles();

private:
    Application* _app = nullptr;

    ImGuiTextFilter _filter;

    Directory* _rootNode = nullptr;

    bool _showDeleteMessage = false;

    bool _deleteFileAccepted = false;

    File* _deleteFile = nullptr;

    Directory* _deleteDir = nullptr;

    FileTree* _fileTree = nullptr;

    std::string _dragPath = "";

    uint _dragUID = 0;

    SDL_Window* _window = nullptr;

    bool _isWindowFocus = false;

    int _itemWidth = 80;

    int _itemHeight = 80;

    // Multi seleccion
    std::vector<SelectableFile*> _selectedFiles;

    // Variables for reimport 
    bool _reimportRequest = false;

    uint _reimportCounter = 100;

    // ChangeFileName
    bool _openChangeNamePanel = false;

    // Create Folder
    bool _openCreateFolderPanel = false;

    // Create Script
    bool _openCreateScriptPanel = false;

    //Create Shader
    bool _openCreateShaderPanel = false;

    //Create Material
    bool _openCreateMaterialPanel = false;

    std::string _temporalName = "default";

    // icons 
    uint _fileImageID = 0;
    uint _folderImageID = 0;
    uint _modelImageID = 0;
    uint _meshImageID = 0;
    uint _sceneImageID = 0;
    uint _textureImageID = 0;
    uint _cppImageID = 0;
    uint _hImageID = 0;
};