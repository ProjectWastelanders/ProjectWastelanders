#ifndef SHADER_H
#define SHADER_H

#include <sstream>
#include <iostream>
#include <map>

#include "ShaderData.h"

class Uniform;

class Shader
{
public:
   
    Shader();
    Shader(std::string shaderPath);
    
    void Clear();
   /* Shader(std::string& vertexPath, std::string& fragmentPath);
    Shader(std::string&& vertexPath, std::string&& fragmentPath);*/
    void Recompile(std::string shaderPath);
    void Bind();
   
    // Uniform Setters
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetDouble(const std::string& name, double value) const;
    void SetFloat2v(const std::string& name, const float* value) const;
    void SetFloat3v(const std::string& name, const float* value) const;
    void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void SetFloat4v(const std::string& name, const float* value) const;
    void SetMatFloat4v(const std::string& name, const float* value, int num = 1, bool transpose = false) const;
    void SetTexture(const std::string& name, uint id, int layer);
    void SetIntv(const std::string& name, int* value, int count);

    void UniformParser(std::vector<Uniform*>& vec);

    ShaderData data;

private:
    void RetriveShader(const std::string& shaderPath);
    void CompileShader();
   
    void VertexShaderCheck();

    //uint CompileShader(const std::string& source, uint type);
    static std::map<std::string, uint> loadedShaders;
};

#endif
