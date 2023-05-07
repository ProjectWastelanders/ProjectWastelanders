#include "Headers.h"
#include "TextureImporter.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ModuleFiles.h"
#include "ModuleResourceManager.h"

std::string TextureImporter::ImportImage(const std::string& fileName, char* buffer, uint size)
{
	std::string ddsFilePath = "Resources/Textures/" + std::to_string(HelloUUID::GenerateUUID()) + ".dds";

	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

	ILuint imgSize;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use
	imgSize = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer

	if (imgSize > 0)
	{
		data = new ILubyte[imgSize]; // allocate data buffer
		ilSaveL(IL_DDS, data, imgSize); // Save to buffer with the ilSaveIL function

		ModuleFiles::S_Save(ddsFilePath, (char*)data, imgSize, false);

		RELEASE_ARRAY(data);
	}

	ilDeleteImages(1, &ImgId);
	return ddsFilePath;
}

void TextureImporter::Load(char* buffer, int size, ResourceTexture* resource)
{
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

	uint error = ilGetError();

	resource->width = ilGetInteger(IL_IMAGE_WIDTH);
	resource->height = ilGetInteger(IL_IMAGE_HEIGHT);
	resource->name = resource->resourcePath;

	ILubyte* bytes = ilGetData();

	for (int i = 0; i < resource->height; i++)
	{
		if (resource->isTransparent)
			break;
		for (int j = 0; j < resource->width; j++)
		{
			if ((int)bytes[(i * resource->width + j) * 4 + 3] != 255)
			{
				resource->isTransparent = true;
				break;
			}
		}
	}

	glGenTextures(1, &resource->OpenGLID);

	//TODO: Generate mipmaps and use best settings
	glBindTexture(GL_TEXTURE_2D, resource->OpenGLID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, resource->width, resource->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &ImgId);
	glBindTexture(GL_TEXTURE_2D, 0);
}

ResourceTexture* TextureImporter::WhiteImage()
{
	if (ModuleResourceManager::resources[WHITE_RESOURCE_UID]->referenceCount != 0)
	{
		ModuleResourceManager::resources[WHITE_RESOURCE_UID]->referenceCount++;
		return (ResourceTexture*)ModuleResourceManager::resources[WHITE_RESOURCE_UID];
	}

	GLubyte whiteImage[240][240][4];
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 240; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			whiteImage[i][j][0] = (GLubyte)255;
			whiteImage[i][j][1] = (GLubyte)255;
			whiteImage[i][j][2] = (GLubyte)255;
			whiteImage[i][j][3] = (GLubyte)255;
		}
	}

	uint textureID = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 240, 240,
		0, GL_RGBA, GL_UNSIGNED_BYTE, whiteImage);

	ResourceTexture* checkersResource = (ResourceTexture*)ModuleResourceManager::resources[WHITE_RESOURCE_UID];
	checkersResource->OpenGLID = textureID;
	checkersResource->name = "White";
	checkersResource->width = checkersResource->height = 240;

	ModuleResourceManager::resources[WHITE_RESOURCE_UID]->referenceCount++;

	return (ResourceTexture*)ModuleResourceManager::resources[WHITE_RESOURCE_UID];
}

ResourceTexture* TextureImporter::CheckerImage()
{
	if (ModuleResourceManager::resources[CHECKERS_RESOURCE_UID]->referenceCount != 0)
	{
		//Doesn't need reference count
		return (ResourceTexture*)ModuleResourceManager::resources[CHECKERS_RESOURCE_UID];
	}

	GLubyte checkerImage[240][240][4];
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 240; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	uint textureID = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 240, 240,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	ResourceTexture* checkersResource = (ResourceTexture*)ModuleResourceManager::resources[CHECKERS_RESOURCE_UID];
	checkersResource->OpenGLID = textureID;
	checkersResource->name = "Checkers";
	checkersResource->width = checkersResource->height = 240;

	ModuleResourceManager::resources[CHECKERS_RESOURCE_UID]->referenceCount++;

	return (ResourceTexture*)ModuleResourceManager::resources[CHECKERS_RESOURCE_UID];
}

uint TextureImporter::LoadEditorDDS(char* buffer, int size)
{
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		LOG("Error loading image: %s", ilutGetString(ilGetError()));
	}

	uint error = ilGetError();

	ILubyte* bytes = ilGetData();

	uint ret = 0;

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	glGenTextures(1, &ret);

	//TODO: Generate mipmaps and use best settings
	glBindTexture(GL_TEXTURE_2D, ret);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &ImgId);
	glBindTexture(GL_TEXTURE_2D, 0);

	return ret;
}
