#pragma once

class ResourceTexture;

#define CHECKERS_RESOURCE_UID 240240
#define WHITE_RESOURCE_UID 241241

class TextureImporter
{
public:
	//TODO: The system to check whether an image has already been loaded or not should be inside the ResourceManager, not here.

	// Loads an image file and saves it as DDS inside Library.
	static std::string ImportImage(const std::string& filename, char* buffer, uint size);

	// Loads a texture from DDS file to OpenGL and returns its ID.
	static void Load(char* buffer, int size, ResourceTexture* resource);

	static ResourceTexture* CheckerImage();
	static ResourceTexture* WhiteImage();

	static uint LoadEditorDDS(char* buffer, int size);
};

