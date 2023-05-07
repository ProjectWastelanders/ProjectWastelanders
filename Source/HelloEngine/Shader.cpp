#include "Headers.h"
#include "Shader.h"
#include <memory>
#include "Console.h"
#include "ModuleFiles.h"
#include "Uniform.h"

std::map<std::string, uint> Shader::loadedShaders;

// We use this method to open the shader because it is easier to format the given file inside a single string this way.
static std::string OpenShader(const std::string& path)
{
	std::ifstream stream(path);

	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
	{
		ss << line << "\n";
	}

	return ss.str();
}

Shader::Shader()
{
}

Shader::Shader(std::string shaderPath)
{
	// Get shader name 
	std::string shaderName = ModuleFiles::S_GetFileName(shaderPath, false);

	if (loadedShaders.find(shaderName) != loadedShaders.end())
	{
		this->data.ID = loadedShaders[shaderName];
		return;
	}
	data.name = shaderName;

	RetriveShader(shaderPath);

	CompileShader();
}

void Shader::Clear()
{
	glDeleteShader(data.ID);
}

void Shader::RetriveShader(const std::string& shaderPath)
{
	const char* vDefine = "#define VERTEX_PROGRAM\n";
	const char* fDefine = "#define FRAGMENT_PROGRAM\n";

	std::ifstream shaderFile;

	//Ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//Open File
		shaderFile.open(shaderPath.c_str());
		std::stringstream shaderStream;

		//Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		//Close file handlers
		shaderFile.close();

		//Convert stream into string
		std::string fullFile = shaderStream.str();

		//Gets version line
		std::string version = fullFile.substr(0, fullFile.find("\n") + 1);
		std::size_t i = fullFile.find(version);
		fullFile.erase(i, version.length());

		//Code
		data.vertexCode = version + vDefine + fullFile;
		data.fragmentCode = version + fDefine + fullFile;
	}
	catch (std::ifstream::failure e)
	{
		Console::S_Log("ERROR: Shader file not succesfully read.", LogType::ERR);
	}
}

void Shader::CompileShader()
{
	const char* vShaderCode = data.vertexCode.c_str();
	const char* fShaderCode = data.fragmentCode.c_str();

	//Checks
	VertexShaderCheck();

	uint vertex, fragment;
	int success;
	char infoLog[512];
	std::string err;

	//Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//Print compiler errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		err = "ERROR::SHADER_VERTEX::COMPILATION_FAILED\n ";
		err.append(infoLog);
		Console::S_Log(err, LogType::ERR);
	}

	//Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//Print compiler errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		err = "ERROR::SHADER_FRAGMENT::COMPILATION_FAILED\n ";
		err.append(infoLog);
		Console::S_Log(err, LogType::ERR);
	}

	//Shader Program
	data.ID = glCreateProgram();
	glAttachShader(data.ID, vertex);
	glAttachShader(data.ID, fragment);
	//glProgramParameteri(ID, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE); //Allows getting the compiled binary
	glLinkProgram(data.ID);
	//Print Linking errors if any
	glGetProgramiv(data.ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(data.ID, 512, NULL, infoLog);
		err = "ERROR::SHADER_PROGRAM::LINKING_FAILED\N ";
		err.append(infoLog);
		Console::S_Log(err, LogType::ERR);
	}
	else
	{
		err = "SUCCESS: Shader '";
		err += data.name;
		err += "' is compiled!";
		Console::S_Log(err, LogType::INFO);

		//UniformParser();
	}

	//Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Recompile(std::string shaderPath)
{
	ShaderData previousData = data;
	Clear();	

	RetriveShader(shaderPath);
	CompileShader();
}

void Shader::VertexShaderCheck()
{
	//String used to check if a shader is meant to be used as an Instanced Shader.
	std::string instanced = "layout (location = 1) in mat4 model;";
	std::string boned = "layout (location = 3) in ivec4 boneIds;";
	std::string engineLight = "uniform DirectionalLight Light_Directional;";

	if (data.vertexCode.find(instanced) != std::string::npos)
		data.isIstanced = true;

	if (data.vertexCode.find(boned) != std::string::npos)
		data.isBoned = true;

	if (data.fragmentCode.find(engineLight) != std::string::npos)
	{
		data.hasEngineLight = true;

		//Get max number of lights the shader will be using.
		std::string spotLights = "const uint MAX_SPOT";
		std::string pointLights = "const uint MAX_POINT";

		if (data.fragmentCode.find(spotLights) != std::string::npos)
			data._maxSpotLights = 32;

		if (data.fragmentCode.find(pointLights) != std::string::npos)
			data._maxSpotLights = 32;
	}
}

void Shader::UniformParser(std::vector<Uniform*>& vec)
{
	if (data.ID == 0)
	{
		Console::S_Log("Trying to Parse a missing Shader. Try ReImporting all Shaders!", LogType::ERR);
		return;
	}

	GLint maxNameLen, count;

	glGetProgramiv(data.ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);
	glGetProgramiv(data.ID, GL_ACTIVE_UNIFORMS, &count);

	
	UniformData uni;
	std::vector<GLchar>uniName(maxNameLen, 0);

	int layerCount = 0;
	for (GLint i = 0; i < count; ++i)
	{
		uni.index = i;
		glGetActiveUniform(data.ID, i, maxNameLen, &uni.read, &uni.size, &uni.type, uniName.data());

		uni.name = uniName.data();

		//Skip uniforms that doesn't need to be handled dynamically by the engine
		if (uni.name == "view" || uni.name == "projection" || uni.name == "model" || 
			uni.name.find("Light_Directional") != std::string::npos ||
			uni.name.find("Light_Spot") != std::string::npos ||
			uni.name.find("Light_Point") != std::string::npos) continue;

		switch (uni.type)
		{
			case GL_BOOL: vec.push_back((Uniform*) new UniBool(uni)); break;
			case GL_INT: vec.push_back((Uniform*) new UniInt(uni)); break;
			case GL_UNSIGNED_INT: vec.push_back((Uniform*) new UniUInt(uni)); break;
			case GL_FLOAT: vec.push_back((Uniform*) new UniFloat(uni)); break;
			case GL_FLOAT_VEC2: vec.push_back((Uniform*) new UniFloat2(uni)); break;
			case GL_FLOAT_VEC3: vec.push_back((Uniform*) new UniFloat3(uni)); break;
			case GL_FLOAT_VEC4: vec.push_back((Uniform*) new UniFloat4(uni)); break;
			case GL_DOUBLE: vec.push_back((Uniform*) new UniDouble(uni)); break;
			case GL_SAMPLER_2D: 
				if (uni.name.find("shadowMap") != std::string::npos)
				{
					vec.push_back((Uniform*) new UniSampler2D(uni, 31));
					break;
				}

				vec.push_back((Uniform*) new UniSampler2D(uni, layerCount));
				layerCount++;
				break;
			case GL_FLOAT_MAT4: vec.push_back((Uniform*) new UniFloat4x4(uni)); break;
		}
	}
}

void Shader::Bind()
{
	glUseProgram(data.ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(data.ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(data.ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(data.ID, name.c_str()), value);
}

void Shader::SetDouble(const std::string& name, double value) const
{
	glUniform1d(glGetUniformLocation(data.ID, name.c_str()), value);
}

void Shader::SetFloat2v(const std::string& name, const float* value) const
{
	glUniform2fv(glGetUniformLocation(data.ID, name.c_str()), 1, value);
}

void Shader::SetFloat3v(const std::string& name, const float* value) const
{
	glUniform3fv(glGetUniformLocation(data.ID, name.c_str()), 1, value);
}

void Shader::SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(data.ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::SetFloat4v(const std::string& name, const float* value) const
{
	glUniform4fv(glGetUniformLocation(data.ID, name.c_str()), 1, value);
}

void Shader::SetMatFloat4v(const std::string& name, const float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(data.ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetTexture(const std::string& name, uint id, int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_2D, id);
	SetInt(name, layer);
}

