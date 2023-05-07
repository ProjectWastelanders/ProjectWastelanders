#include "Headers.h"
#include "Uniform.h"

#include "ModuleResourceManager.h"
#include "TextureImporter.h"

/*Constructors and Deconstructors*/

UniSampler2D::UniSampler2D(json & j)
{
	SetJSON(j);
	int UID = j["Value"];
	layer = j["Layer"];
	if (UID == -1)
	{
		data.value = nullptr;
		return;
	}
	ResourceTexture* texture = (ResourceTexture*)ModuleResourceManager::S_LoadResource(UID);
	data.value = texture;
}

UniSampler2D::~UniSampler2D()
{
	ResourceTexture* texture = static_cast<ResourceTexture*>(data.value);

	if (texture != nullptr) texture->Dereference();
}

/*Special Methods*/
void UniSampler2D::Unbind()
{
	ResourceTexture* texture = static_cast<ResourceTexture*>(data.value);

	if (texture)
	{
		glActiveTexture(GL_TEXTURE0 + layer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}	
}

/*Variable Set*/

void UniBool::SetVariable()
{
	data.strType = "bool";
	bool* b = new bool(false);
	data.value = b;
}

void UniInt::SetVariable()
{
	data.strType = "int";
	int* i = new int(0);
	data.value = i;
}

void UniUInt::SetVariable()
{
	data.strType = "uint";
	uint* ui = new uint(0);
	data.value = ui;
}

void UniFloat::SetVariable()
{
	data.strType = "float";
	float* f = new float(0.0f);
	data.value = f;
}

void UniFloat2::SetVariable()
{
	data.strType = "vec2";
	float2* f2 = new float2();
	data.value = f2;
}

void UniFloat3::SetVariable()
{
	data.strType = "vec3";
	float3* f3 = new float3();
	data.value = f3;
}

void UniFloat4::SetVariable()
{
	data.strType = "vec4";
	float4* f4 = new float4();
	data.value = f4;
}

void UniDouble::SetVariable()
{
	data.strType = "double";
	double* d = new double(1.0);
	data.value = d;
}

void UniSampler2D::SetVariable()
{
	data.strType = "sampler2D";
	ResourceTexture* resTex = nullptr;
	data.value = resTex;
}

void UniFloat4x4::SetVariable()
{
	data.strType = "mat4";
	float4x4* f44 = new float4x4(float4x4::identity);
	data.value = f44;
}

/*UPDATE / Value out*/

void UniBool::Update(Shader& shader)
{
	shader.SetBool(data.name, *static_cast<bool*>(data.value));
}

void UniInt::Update(Shader& shader)
{
	shader.SetInt(data.name, *static_cast<int*>(data.value));
}

void UniUInt::Update(Shader& shader)
{
	shader.SetInt(data.name, *static_cast<int*>(data.value));
}

void UniFloat::Update(Shader& shader)
{
	shader.SetFloat(data.name, *static_cast<float*>(data.value));
}

void UniFloat2::Update(Shader& shader)
{
	shader.SetFloat2v(data.name, &static_cast<float2*>(data.value)->At(0));
}

void UniFloat3::Update(Shader& shader)
{
	float3 f3 = float3(static_cast<float3*>(data.value)->At(0));

	shader.SetFloat3v(data.name, &static_cast<float3*>(data.value)->At(0));
}

void UniFloat4::Update(Shader& shader)
{
	shader.SetFloat4v(data.name, &static_cast<float4*>(data.value)->At(0));
}

void UniDouble::Update(Shader& shader)
{
	shader.SetDouble(data.name, *static_cast<double*>(data.value));
}

void UniSampler2D::Update(Shader& shader)
{
	ResourceTexture* texture = static_cast<ResourceTexture*>(data.value);

	if (texture)
		shader.SetTexture(data.name, texture->OpenGLID, layer);
	else
	{
		texture = TextureImporter::WhiteImage();
		shader.SetTexture(data.name, texture->OpenGLID, layer);
	}	
}

void UniFloat4x4::Update(Shader& shader)
{
	shader.SetMatFloat4v(data.name, &static_cast<float4x4*>(data.value)->v[0][0]);
}


/*HANDLE IMGUI INSPECTOR*/
#ifdef STANDALONE
void UniBool::GUI()
{
	ImGui::Checkbox(data.name.c_str(), static_cast<bool*>(data.value));
}

void UniInt::GUI()
{
	ImGui::InputInt(data.name.c_str(), static_cast<int*>(data.value));
}

void UniUInt::GUI()
{
	ImGui::InputInt(data.name.c_str(), static_cast<int*>(data.value), 0, 4294967295);
}

void UniFloat::GUI()
{
	if (data.name == "deltaTime" || data.name == "DeltaTime" || data.name == "dt") return;
	ImGui::InputFloat(data.name.c_str(), static_cast<float*>(data.value));
}

void UniFloat2::GUI()
{
	ImGui::InputFloat2(data.name.c_str(), &static_cast<float2*>(data.value)->At(0));
}

void UniFloat3::GUI()
{
	if (data.name.find("Colour") || data.name.find("Color") || data.name.find("color") || data.name.find("colour"))
	{
		std::string popName = "ColorPicker##" + data.index;
		float4 color = { *static_cast<float3*>(data.value), 1.0f };
		if (ImGui::ColorButton(data.name.c_str(), (ImVec4&)color, 0, { 50, 50 }))
		{
			ImGui::OpenPopup(popName.c_str());
		}
		ImGui::SameLine();
		ImGui::Text(data.name.c_str());

		if (ImGui::BeginPopup(popName.c_str()))
		{
			ImGui::ColorPicker3(data.name.c_str(), (float*) static_cast<float3*>(data.value), 0 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			ImGui::EndPopup();
		}

	}
	else ImGui::InputFloat3(data.name.c_str(), &static_cast<float3*>(data.value)->At(0));
}

void UniFloat4::GUI()
{
	if (!data.name.find("Colour") || !data.name.find("Color") || !data.name.find("color") || !data.name.find("colour"))
	{
		float4 color = *static_cast<float4*>(data.value);
		if (ImGui::ColorButton(data.name.c_str(), (ImVec4&)color, 0, { 50, 50 }))
		{
			ImGui::OpenPopup("ColorPicker");
		}
		ImGui::SameLine();
		ImGui::Text(data.name.c_str());

		if (ImGui::BeginPopup("ColorPicker"))
		{
			ImGui::ColorPicker4(data.name.c_str(), (float*) static_cast<float4*>(data.value), 0 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

			ImGui::EndPopup();
		}
	}
	else ImGui::InputFloat4(data.name.c_str(), &static_cast<float4*>(data.value)->At(0));
}

void UniDouble::GUI()
{
	ImGui::InputDouble(data.name.c_str(), static_cast<double*>(data.value));
}

void UniSampler2D::GUI()
{
	ResourceTexture* texture = static_cast<ResourceTexture*>(data.value);
	
	int id = -1;
	if (texture) id = texture->OpenGLID;

	ImGui::ImageButton((ImTextureID)id, { 75, 75 });

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture"))
		{
			const uint* drop = (uint*)payload->Data;

			if (texture != nullptr) texture->Dereference();
			texture = (ResourceTexture*)ModuleResourceManager::S_LoadResource(*drop);

			data.value = texture;
		}
	}
	ImGui::SameLine();
	ImGui::Text(data.name.c_str());

	if (texture)
	{
		ImGui::SameLine();
		ImGui::Text("%s", texture->debugName.c_str());
		std::string txt = "Size: " + std::to_string(texture->width) + " x " + std::to_string(texture->height);
		ImGui::Text(txt.c_str());
		txt = "Texture ID: " + std::to_string(texture->OpenGLID);
		ImGui::Text(txt.c_str());
		txt = "Layer: " + std::to_string(layer);
		ImGui::Text(txt.c_str());
	}

	//ImGui::InputInt(data.name.c_str(), static_cast<int*>(data.value), 0, 31);
}

void UniFloat4x4::GUI()
{
	std::string aux;
	float4x4* f44 = static_cast<float4x4*>(data.value);
	aux = "1: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[0][0]);
	aux = "2: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[1][0]);
	aux = "3: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[2][0]);
	aux = "4: " + data.name;
	ImGui::InputFloat4(aux.c_str(), &f44->v[3][0]);
}
#endif

/* GET JSON */

void Uniform::GetJSON(json& j)
{
	json _j;

	_j["Name"] = data.name;
	_j["Type"] = data.type;
	_j["String Type"] = data.strType;
	_j["Index"] = data.index;
	_j["Read"] = data.read;
	_j["Size"] = data.size;

	GetJSONUnique(_j);

	j.push_back(_j);
}

void UniBool::GetJSONUnique(json& _j)
{
	_j["Value"] = *static_cast<bool*>(data.value);
}

void UniInt::GetJSONUnique(json& _j)
{
	_j["Value"] = *static_cast<int*>(data.value);
}

void UniUInt::GetJSONUnique(json& _j)
{
	_j["Value"] = *static_cast<uint*>(data.value);
}

void UniFloat::GetJSONUnique(json& _j)
{
	_j["Value"] = *static_cast<float*>(data.value);
}

void UniFloat2::GetJSONUnique(json& _j)
{
	float2 f2 = *static_cast<float2*>(data.value);
	json jj;

	jj[0] = f2[0];
	jj[1] = f2[1];

	_j["Value"] = jj;
}

void UniFloat3::GetJSONUnique(json& _j)
{
	float3 f3 = *static_cast<float3*>(data.value);
	json jj;

	jj[0] = f3[0];
	jj[1] = f3[1];
	jj[2] = f3[2];

	_j["Value"] = jj;
}

void UniFloat4::GetJSONUnique(json& _j)
{
	float4 f4 = *static_cast<float4*>(data.value);
	json jj;

	jj[0] = f4[0];
	jj[1] = f4[1];
	jj[2] = f4[2];
	jj[3] = f4[3];

	_j["Value"] = jj;
}

void UniDouble::GetJSONUnique(json& _j)
{
	_j["Value"] = *static_cast<double*>(data.value);
}

void UniSampler2D::GetJSONUnique(json& _j)
{
	ResourceTexture* texture = static_cast<ResourceTexture*>(data.value);

	if (texture != nullptr)
	{
		_j["Value"] = texture->UID;
	}
	else
	{
		_j["Value"] = -1;
	}
	_j["Layer"] = layer;
}

void UniFloat4x4::GetJSONUnique(json& _j)
{
	float4x4 f4x4 = *static_cast<float4x4*>(data.value);
	json jj;

	int k = 0;
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			jj[k] = f4x4[x][y];
			++k;
		}
	}

	_j["Value"] = jj;
}