#pragma once

struct LightMap;
class ResourceShader;
class Uniform;

#include "json.hpp"

using json = nlohmann::json;

class Material
{
public:
	Material();
	Material(uint UID);
	~Material();

	void UpdateBones(std::vector<float4x4>& bones);
	void UpdateLights();
	void Update(const float* view, const float* projection, const float* model);
	void UpdateInstanced(const float* view, const float* projection);

	void UnbindAllTextures();

	void Clear();

	ResourceShader* GetShader() { return shader; }
	bool SetShader(uint UID);

	void Save(json& j);
	void LoadJSON(std::string filePath);

	void CheckVersion();
private:
	bool HandleKeyUniforms(Uniform* uni);

	void CleanUniforms();

public:
	std::vector<Uniform*> uniforms;

private:
	ResourceShader* shader = nullptr;

	int shaderVersion;
	std::vector<int> usedTexturesID;
};

