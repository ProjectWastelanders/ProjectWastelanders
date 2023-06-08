#include "Headers.h"
#include "ModuleLayers.h"
#include "API_Material.h"
#include "TextureComponent.h"
#include "MeshRenderComponent.h"
#include "ModuleResourceManager.h"

API::API_Material::API_Material()
{
}

API::API_Material::~API_Material()
{
}

API::API_GameObject API::API_Material::GetGameObject()
{
	if (!_material)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR API_Material. GetGameObject()");
		return *ModuleLayers::emptyAPIGameObject;
	}
	API_GameObject returnGO;
	returnGO.SetGameObject(_material->GetGameObject());
	return returnGO;
}

void API::API_Material::ChangeAlbedoTexture(uint textureUID)
{
	if (!_material)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR API_Material. ChangeAlbedoTexture()");
		return;
	}
	_material->ChangeTexture((ResourceTexture*)ModuleResourceManager::S_LoadResource(textureUID));
}

bool API::API_Material::IsTextureNull()
{
	if (!_material)
	{
		Engine::Console::S_Log("Trying to acces a NULLPTR API_Material. IsTextureNull()");
		return true;
	}
	return _material->GetResourceUID() == -1.0f;
}

TextureComponent* API::API_Material::GetComponent()
{
    return _material;
}

void API::API_Material::SetComponent(TextureComponent* component)
{
	_material = component;
}
