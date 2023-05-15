#include "Headers.h"
#include "TextureManager.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "TextureImporter.h"
#include "ModuleLayers.h"
#include "LayerEditor.h"
#include "ImWindowConfiguration.h"


std::map<uint, Texture> TextureManager::loadedTextures;
std::map<std::string, uint> TextureManager::usedPaths;
std::vector<TextureManager::BindedTextureInfo> TextureManager::bindedTexturesInfo;
uint TextureManager::bindedTextures = 0;
int TextureManager::bindTexturesArray[32];

float TextureManager::BindTexture(uint texture)
{
	for (int i = 0; i < bindedTexturesInfo.size(); i++)
	{
		if (bindedTexturesInfo[i].TextureID == texture)
		{
			return bindedTexturesInfo[i].OpenGLSlotID; // If binding texture has already been binded, return the OpenGl slot
		}
	}

	if (GL_TEXTURE0 + bindedTextures == GL_TEXTURE31) // If we loaded 32 textures already
		return -1;

	glActiveTexture(GL_TEXTURE0 + bindedTextures);
	glBindTexture(GL_TEXTURE_2D, texture);

	bindedTexturesInfo.push_back({ texture, bindedTextures });

	/*if (!ModuleEditor::configuration->renderConfigs["texture2D"].first)
	{
		bindedTextures++;
		return -1.0f;
	}*/
	bindTexturesArray[bindedTextures] = bindedTextures;
	return bindedTextures++;
}

void TextureManager::UnBindTextures()
{
	bindedTextures = 0;
	bindedTexturesInfo.clear();
	for (int i = 0; i < 32; i++)
	{
		glBindTexture(GL_TEXTURE0 + i, 0); // Unbind every texture
		bindTexturesArray[i] = 0;
		/*
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind every texture
		*/
	}
}
