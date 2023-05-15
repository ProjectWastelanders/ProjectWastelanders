#pragma once

#include "Texture.h"

class TextureManager
{
public:

	struct BindedTextureInfo
	{
		BindedTextureInfo(uint tID, uint oglID) : TextureID(tID), OpenGLSlotID(oglID){}
		uint TextureID;
		uint OpenGLSlotID;
	};

	static float BindTexture(uint texture);
	static void UnBindTextures();

	static uint bindedTextures;
	static std::map<uint, Texture> loadedTextures;
	static std::map<std::string, uint> usedPaths;
	static std::vector<BindedTextureInfo> bindedTexturesInfo;
	static int bindTexturesArray[32];
};

