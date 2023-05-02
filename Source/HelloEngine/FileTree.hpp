#pragma once

#include <string>
#include <vector>

#include "Application.h"
#include "ModuleResourceManager.h"

struct Directory;

enum class FileType
{
	File,
	Directory,
};

struct SelectableFile {
public:
	virtual bool IsSelected() const = 0;
	virtual void SetSelected(bool selected) = 0;
	FileType GetFileType() { return type; }
protected:
	bool selected = false;
	FileType type;
};

struct File : public SelectableFile
{
	File(std::string path, std::string name, Directory* parent) :path(path), name(name), parent(parent)
	{
		type = FileType::File;

		// Check if this file has a .helloMeta file with the same name.
		if (ModuleFiles::S_CheckMetaExist(path))
		{
			metaPath = path + ".helloMeta";
			metaFile = ModuleFiles::S_LoadMeta(metaPath);

			
			// Check if this meta file has a different modify time than the file.
#ifdef STANDALONE
			unsigned long long modifyTime = ModuleFiles::S_CheckFileLastModify(path);
			if (metaFile.lastModified != modifyTime)
				Reimport();
#endif // STANDALONE
		}
		else
		{
			// If the resource type of this file is not undefined:
			// iF it doesn't have a meta file, create one by using the ImportFile method inside ModuleResourceManager
			if (ModuleFiles::S_GetResourceType(path) == ResourceType::UNDEFINED ||
				ModuleFiles::S_GetResourceType(path) == ResourceType::SCENE ||
				ModuleFiles::S_GetResourceType(path) == ResourceType::ANIMATION)
				return;

			ModuleResourceManager::S_ImportFile(path);

			metaPath = path + ".helloMeta";
			metaFile = ModuleFiles::S_LoadMeta(metaPath);

			ModuleResourceManager::S_CreateResource(metaFile);
		}
	};

	bool IsSelected() const override
	{
		return selected;
	}

	void SetSelected(bool selected) override
	{
		this->selected = selected;
	}

	/// <summary>
	/// Force ReImport the file
	/// </summary>
	void Reimport()
	{
		if (ModuleResourceManager::resources.count(metaFile.UID) != 0)
		{
			ModuleResourceManager::S_ReImportFile(path, metaFile.type);
		}
		else
		{
			metaFile = ModuleFiles::S_LoadMeta(metaPath);
			ModuleResourceManager::S_CreateResource(metaFile);
			ModuleResourceManager::S_ReImportFile(path, metaFile.type);
		}
		if (metaFile.type != ResourceType::UNDEFINED)
			metaFile = ModuleFiles::S_LoadMeta(metaPath);
	}

private:
	File() {};

public:
	std::string path;
	std::string metaPath = "none";
	std::string name;
	Directory* parent = nullptr;
	MetaFile metaFile;
	bool pressed = false;
};

struct Directory : public SelectableFile
{
	Directory(std::string path, std::string name, Directory* parent) :path(path), name(name), parent(parent)
	{
		type = FileType::Directory;
	};
	~Directory()
	{
		for (size_t i = 0; i < directories.size(); i++)
		{
			RELEASE(directories[i]);
		}
	}

	bool IsSelected() const override {
		return selected;
	}

	void SetSelected(bool selected) override
	{
		this->selected = selected;
	}

	std::vector<Directory*> directories;
	std::vector<File> files;
	std::string path;
	std::string name;
	Directory* parent = nullptr;
};

class FileTree
{
public:
	FileTree() {};

	~FileTree()
	{
		RELEASE(_root);
	}

	bool GetRootDir(Directory*& dir)
	{
		if (_root)
		{
			dir = _root;
			true;
		}
		return false;
	}

	void SetNewRoot(Directory* root)
	{
		if (_root)
		{
			RELEASE(_root);
		}
		_root = root;
	}

	std::string GetRootPath()
	{
		return _rootPath;
	}

public:
	Directory* _currentDir = nullptr;

private:
	Directory* _root = nullptr;

	std::string _rootPath;
};
