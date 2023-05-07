#pragma once

#include <string>
#include <vector>
#include <cassert>
#include "MathGeoLib.h"
#include "ModuleXML.h"
#include "Mesh.h"
#include "json.hpp"
#include "UUID.h"

using json = nlohmann::json;

struct ModelNode
{
	std::string name = "Node";
	std::string meshPath = "N"; // By default, this path is set to "N". Paths with "N" will be seen as Null.
	uint resourceMaterialUID = 0;
	float3 position = float3::zero;
	float3 rotation = float3::zero;
	float3 scale = {1,1,1};
	int childrenNum = 0;
	std::vector<ModelNode> children;
	uint UID = 0;

	void Reset()
	{
		UID = 0;
		children.clear();
		childrenNum = 0;
		position = float3::zero;
		rotation = float3::zero;
		scale = { 1,1,1 };
		resourceMaterialUID = 0;
		name = "Node";
		meshPath = "N";
	}

	void WriteToJSON(std::string filename = "")
	{
		json file;

		WriteToParent(file, 0, true);

		std::string data = file.dump();

		ModuleFiles::S_Save(filename, &data[0], data.size(), false);
	}

	void ReadFromJSON(std::string filePath)
	{
		char* data = nullptr;
		ModuleFiles::S_Load(filePath, &data);

		if (data == nullptr)
			return;

		json file = json::parse(data);

		std::vector<std::string> childrenUIDs = file["0"]["children"];

		name = file["0"]["name"];
		meshPath = file["0"]["meshPath"];
		resourceMaterialUID = file["0"]["resourceUID"];

		std::vector<float> positions = file["0"]["position"];
		position = { positions[0], positions[1], positions[2] };

		std::vector<float> rotations = file["0"]["rotation"];
		rotation = { rotations[0], rotations[1], rotations[2] };

		std::vector<float> scales = file["0"]["scale"];
		scale = { scales[0], scales[1], scales[2] };

		UID = 0;
		childrenNum = childrenUIDs.size();

		for (int i = 0; i < childrenUIDs.size(); i++)
		{
			ModelNode newNode;
			newNode.ReadFromParent(file, childrenUIDs[i]);
			children.push_back(newNode);
		}
		RELEASE(data);
	}

private:
	std::string WriteToParent(json& file, uint parentUID, bool isRoot = false)
	{
		if (!isRoot)
			UID = HelloUUID::GenerateUUID();
		else
			UID = 0;

		std::string stringUID = std::to_string(UID);

		file[stringUID]["name"] = name;
		file[stringUID]["meshPath"] = meshPath;
		file[stringUID]["position"] = { position.x, position.y, position.z };
		file[stringUID]["rotation"] = { rotation.x, rotation.y, rotation.z };
		file[stringUID]["scale"] = { scale.x, scale.y, scale.z };
		file[stringUID]["childrenNum"] = children.size();
		file[stringUID]["UID"] = UID;
		file[stringUID]["parentUID"] = parentUID;
		file[stringUID]["resourceUID"] = resourceMaterialUID;

		std::vector<std::string> childrenUIDs; 

		for (int i = 0; i < children.size(); i++)
		{
			childrenUIDs.push_back(children[i].WriteToParent(file, UID));
		}
		file[stringUID]["children"] = childrenUIDs;
		return stringUID;
	}

	void ReadFromParent(json& file, std::string uid)
	{
		std::vector<std::string> childrenUIDs = file[uid]["children"];

		name = file[uid]["name"];
		meshPath = file[uid]["meshPath"];
		resourceMaterialUID = file[uid]["resourceUID"];

		std::vector<float> positions = file[uid]["position"];
		position = { positions[0], positions[1], positions[2] };

		std::vector<float> rotations = file[uid]["rotation"];
		rotation = { rotations[0], rotations[1], rotations[2] };

		std::vector<float> scales = file[uid]["scale"];
		scale = { scales[0], scales[1], scales[2] };

		UID = std::stoul(uid);
		childrenNum = childrenUIDs.size();

		for (int i = 0; i < childrenUIDs.size(); i++)
		{
			ModelNode newNode;
			newNode.ReadFromParent(file, childrenUIDs[i]);
			children.push_back(newNode);
		}
	}
};

struct MeshInfo
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	uint hasTexture = 0; // For now, this only implies a diffuse texture.
	std::map<std::string, BoneData> boneDataMap;

	void Clear()
	{
		vertices.clear();
		indices.clear();
		boneDataMap.clear();
	}

	/// <summary>
	/// Returns path to created binary file.
	/// </summary>
	std::string SaveToBinaryFile(std::string fileName)
	{
		std::string filePath = "Resources/Meshes/" + std::to_string(HelloUUID::GenerateUUID()) + ".hmesh";

		std::string corruptionPrev;
		corruptionPrev.resize(128);

		//Bone map to vectors
		std::string fullMapKeyString = "";
		std::vector<BoneData> boneMapValues;
		for (std::map<std::string, BoneData>::iterator it = boneDataMap.begin(); it != boneDataMap.end(); ++it) {
			//All bone map keys into one long string
			fullMapKeyString += it->first;
			fullMapKeyString += "\n";

			boneMapValues.push_back(it->second);
		}

		uint verticesSize = vertices.size() * sizeof(Vertex);
		uint indicesSize = indices.size() * sizeof(uint);
		uint boneMapValuesSize = boneMapValues.size() * sizeof(BoneData);
		
		uint header[4] = { vertices.size(), indices.size(), fullMapKeyString.size(), boneMapValues.size()};

		uint fileSize = verticesSize + indicesSize + fullMapKeyString.size() + boneMapValuesSize + corruptionPrev.size(); // Vertex + indices + boneMapKeysSize + boneMapKeysValue

		char* fileBuffer = new char[fileSize];
		char* cursor = fileBuffer;

		// Save header
		uint headerSize = sizeof(header);
		memcpy(cursor, header, headerSize);
		cursor += headerSize;

		// Save indices
		memcpy(cursor, &indices[0], indicesSize);
		cursor += indicesSize;

		// Save vertices
		memcpy(cursor, &vertices[0], verticesSize);
		cursor += verticesSize;

		//Save boneDataMap((
		if (fullMapKeyString.size() != 0)
		{
			//Save Keys of boneDataMap
			memcpy(cursor, &fullMapKeyString[0], fullMapKeyString.size());
			cursor += fullMapKeyString.size();

			//Save Values of boneDataMap
			memcpy(cursor, &boneMapValues[0], boneMapValuesSize);
			cursor += boneMapValuesSize;
		}	

		//Data corruption prevention
		// Save vertices
		memcpy(cursor, &corruptionPrev[0], corruptionPrev.size());
		cursor += corruptionPrev.size();

		ModuleFiles::S_Save(filePath, fileBuffer, fileSize, false);

		RELEASE(fileBuffer);

		return filePath;
	}

	void LoadFromBinaryFile(const std::string& filePath)
	{
		std::vector<std::string> boneMapKeys;
		std::vector<BoneData> boneMapValues;

		char* buffer = nullptr;
		uint size = ModuleFiles::S_Load(filePath, &buffer);

		char* cursor = buffer;

		uint header[4];
		uint headerSize = sizeof(uint) * 4;
		memcpy(header, cursor, headerSize);
		cursor += headerSize;

		uint indicesSize = header[1] * sizeof(uint);
		indices.resize(header[1]);
		memcpy(&indices[0], cursor, indicesSize);
		cursor += indicesSize;

		uint verticesSize = header[0] * sizeof(Vertex);
		vertices.resize(header[0]);
		memcpy(&vertices[0], cursor, verticesSize);
		cursor += verticesSize;

		uint boneMapKeysSize = header[2] * sizeof(char);
		if (boneMapKeysSize != 0)
		{
			std::string fullMapKeyString;
			fullMapKeyString.resize(header[2]);
			memcpy(&fullMapKeyString[0], cursor, boneMapKeysSize);
			cursor += boneMapKeysSize;

			//fullMapKeyString into boneMapKeys (Vector)
			std::stringstream ss(fullMapKeyString);
			std::string tmp;
			while (std::getline(ss, tmp, '\n')) {
				boneMapKeys.emplace_back(tmp);
			}


			uint boneMapValuesSize = header[3] * sizeof(BoneData);
			boneMapValues.resize(header[3]);
			memcpy(&boneMapValues[0], cursor, boneMapValuesSize);
			cursor += boneMapValuesSize;
		

			//BoneMap vectors to map.
			for (int i = 0; i < boneMapKeys.size(); ++i)
			{
				boneDataMap.emplace(boneMapKeys[i], boneMapValues[i]);
			}
		}

		RELEASE(buffer);
	}
};

struct AnimatedBone
{
	std::string name = "";

	std::map<float, float3> positions;
	std::map<float, float3> scales;
	std::map<float, Quat> rotations;
	
	float3 GetInterpolatedValue_f3(std::map<float, float3>& map, float animTime) {
		if (map.find(animTime) != map.end())
		{
			return map[animTime];
		}
		else
		{
			std::map<float, float3>::iterator last, next;
			next = map.upper_bound(animTime);

			//Check Error
			if (next == map.begin())
			{
				return map.begin()->second;
			}
			else if(next == map.end()) {
				return map.rbegin()->second;
			}

			last = next;
			last--;

			//Get normalised lerp time
			float normAnimTime = (animTime - last->first) / (next->first - last->first);
			
			return float3::Lerp(last->second, next->second, normAnimTime);
		}
	}

	Quat GetInterpolatedValue_Quat(std::map<float, Quat>& map, float animTime) {
		if (map.find(animTime) != map.end())
		{
			return map[animTime];
		}
		else
		{
			std::map<float, Quat>::iterator last, next;
			next = map.upper_bound(animTime);

			//Check Error
			if (next == map.begin())
			{
				return map.begin()->second;
			}
			else if (next == map.end()) {
				return map.rbegin()->second;
			}

			last = next;
			last--;

			//Get normalised lerp time
			float normAnimTime = (animTime - last->first) / (next->first - last->first);

			return Quat::Slerp(last->second, next->second, normAnimTime);
		}
	}

	float3x4 GetTransform(float animTime) {
		float3x4 matrix = float3x4::identity;

		//Get Interpolated Values on this TIME to build the keyframe info
		if (!positions.empty())
		{
			matrix.SetTranslatePart(GetInterpolatedValue_f3(positions, animTime));
		}
		if (!rotations.empty())
		{
			matrix.SetRotatePart(GetInterpolatedValue_Quat(rotations, animTime));
		}
		if (!scales.empty())
		{
			float3 scale = GetInterpolatedValue_f3(scales, animTime);
			matrix[0][0] *= scale.x;
			matrix[1][1] *= scale.y;
			matrix[2][2] *= scale.z;
		}

		return matrix;
	}
};

struct Animation3D
{
public:
	uint durationTicks = 0;
	uint ticksPerSecond = 0;
	std::vector<AnimatedBone> bones;

	void Clear()
	{
		bones.clear();
	}

	/// Returns path to created binary file.
	std::string SaveToBinaryFile()
	{
		std::string filePath = "Resources/Animations/" + std::to_string(HelloUUID::GenerateUUID()) + ".hanim";
		
		std::string corruptionPrev;
		corruptionPrev.resize(128);

		//Bone names together
		std::string boneNamesString = "";
		
		//Size in bytes of map info in bones
		uint boneMapTotalSize = 0;

		//Get all bone names & map sizes
		for (int i = 0; i < bones.size(); ++i)
		{
			boneNamesString += bones[i].name;
			boneNamesString += "\n";

			boneMapTotalSize += bones[i].positions.size() * (sizeof(float) + sizeof(float3));
			boneMapTotalSize += bones[i].rotations.size() * (sizeof(float) + sizeof(Quat));
			boneMapTotalSize += bones[i].scales.size() * (sizeof(float) + sizeof(float3));
		}

		//Info about [positions, rotations, scales] sizes in bones
		uint boneMapsInfoSize = bones.size() * 3 * sizeof(uint);

		//General Info + Number of bones and size of big String
		uint header[4] = {durationTicks, ticksPerSecond, bones.size(), boneNamesString.size()};

		//Size of header, big string, bone map values, bone info about map sizes, corruption
		uint fileSize = (sizeof(uint) * 4) + (sizeof(char) * boneNamesString.size()) + boneMapTotalSize + boneMapsInfoSize + corruptionPrev.size();
		
		char* fileBuffer = new char[fileSize];
		char* cursor = fileBuffer;

		//Save header
		uint headerSize = sizeof(uint) * 4;
		memcpy(cursor, header, headerSize);
		cursor += headerSize;

		//Save boneNamesString
		memcpy(cursor, &boneNamesString[0], boneNamesString.size());
		cursor += boneNamesString.size();

		//Bones
		for (int i = 0; i < bones.size(); ++i)
		{
			//Map size values
			uint boneHeader[3] = { bones[i].positions.size(), bones[i].rotations.size(), bones[i].scales.size() };
			uint boneHeaderSize = sizeof(uint) * 3;
			memcpy(cursor, boneHeader, boneHeaderSize);
			cursor += boneHeaderSize;

			//Map values
				//Position
			std::vector<float> posKey;
			std::vector<float3> posVal;
			posKey.reserve(bones[i].positions.size());
			posVal.reserve(bones[i].positions.size());
			for (std::map<float, float3>::iterator it = bones[i].positions.begin(); it != bones[i].positions.end(); it++)
			{
				posKey.push_back(it->first);
				posVal.push_back(it->second);
			}
			memcpy(cursor, &posKey[0], bones[i].positions.size() * sizeof(float));
			cursor += bones[i].positions.size() * sizeof(float);
			memcpy(cursor, &posVal[0], bones[i].positions.size() * sizeof(float3));
			cursor += bones[i].positions.size() * sizeof(float3);

				//Rotation
			std::vector<float> rotKey;
			std::vector<Quat> rotVal;
			rotKey.reserve(bones[i].rotations.size());
			rotVal.reserve(bones[i].rotations.size());
			for (std::map<float, Quat>::iterator it = bones[i].rotations.begin(); it != bones[i].rotations.end(); it++)
			{
				rotKey.push_back(it->first);
				rotVal.push_back(it->second);
			}
			memcpy(cursor, &rotKey[0], bones[i].rotations.size() * sizeof(float));
			cursor += bones[i].rotations.size() * sizeof(float);
			memcpy(cursor, &rotVal[0], bones[i].rotations.size() * sizeof(Quat));
			cursor += bones[i].rotations.size() * sizeof(Quat);
				
				//Scale
			std::vector<float> scaKey;
			std::vector<float3> scaVal;
			scaKey.reserve(bones[i].scales.size());
			scaVal.reserve(bones[i].scales.size());
			for (std::map<float, float3>::iterator it = bones[i].scales.begin(); it != bones[i].scales.end(); it++)
			{
				scaKey.push_back(it->first);
				scaVal.push_back(it->second);
			}
			memcpy(cursor, &scaKey[0], bones[i].scales.size() * sizeof(float));
			cursor += bones[i].scales.size() * sizeof(float);
			memcpy(cursor, &scaVal[0], bones[i].scales.size() * sizeof(float3));
			cursor += bones[i].scales.size() * sizeof(float3);
		}

		//Data corruption prevention
		memcpy(cursor, &corruptionPrev[0], corruptionPrev.size());
		cursor += corruptionPrev.size();

		ModuleFiles::S_Save(filePath, fileBuffer, fileSize, false);

		RELEASE(fileBuffer);

		return filePath;
	}

	void LoadFromBinaryFile(const std::string& filePath)
	{
		std::string boneNamesString;

		char* buffer = nullptr;
		ModuleFiles::S_Load(filePath, &buffer);

		char* cursor = buffer;

		//Load headers
		uint header[4];
		uint headerSize = sizeof(uint) * 4;
		memcpy(header, cursor, headerSize);
		cursor += headerSize;

		durationTicks = header[0];
		ticksPerSecond = header[1];

		//Load boneNamesString
		uint boneNamesStringSize = header[3] * sizeof(char);
		boneNamesString.resize(header[3]);
		memcpy(&boneNamesString[0], cursor, boneNamesStringSize);
		cursor += boneNamesStringSize;

		//Load numBones
		bones.reserve(header[2]);

		//Initialize bones with names
		std::stringstream ss(boneNamesString);
		std::string tmp;
		while (std::getline(ss, tmp, '\n')) {
			AnimatedBone aux;
			aux.name = tmp;
			bones.push_back(aux);
		}

		//Read all bones
		for (uint i = 0; i < header[2]; i++) 
		{
			//Map Sizes
			uint boneHeader[3];
			memcpy(boneHeader, cursor, sizeof(uint) * 3);
			cursor += sizeof(uint) * 3;

				//Position
			std::vector<float> posKey;
			std::vector<float3> posVal;
			posKey.resize(boneHeader[0]);
			posVal.resize(boneHeader[0]);
			memcpy(&posKey[0], cursor, boneHeader[0] * sizeof(float));
			cursor += boneHeader[0] * sizeof(float);
			memcpy(&posVal[0], cursor, boneHeader[0] * sizeof(float3));
			cursor += boneHeader[0] * sizeof(float3);

				//Rotation
			std::vector<float> rotKey;
			std::vector<Quat> rotVal;
			rotKey.resize(boneHeader[1]);
			rotVal.resize(boneHeader[1]);
			memcpy(&rotKey[0], cursor, boneHeader[1] * sizeof(float));
			cursor += boneHeader[1] * sizeof(float);
			memcpy(&rotVal[0], cursor, boneHeader[1] * sizeof(Quat));
			cursor += boneHeader[1] * sizeof(Quat);

				//Scale
			std::vector<float> scaKey;
			std::vector<float3> scaVal;
			scaKey.resize(boneHeader[2]);
			scaVal.resize(boneHeader[2]);
			memcpy(&scaKey[0], cursor, boneHeader[2] * sizeof(float));
			cursor += boneHeader[2] * sizeof(float);
			memcpy(&scaVal[0], cursor, boneHeader[2] * sizeof(float3));
			cursor += boneHeader[2] * sizeof(float3);

			//Build Maps
			for (int k = 0; k < posKey.size(); ++k)
			{
				bones[i].positions[posKey[k]] = posVal[k];
			}

			for (int k = 0; k < rotKey.size(); ++k)
			{
				bones[i].rotations[rotKey[k]] = rotVal[k];
			}

			for (int k = 0; k < scaKey.size(); ++k)
			{
				bones[i].scales[scaKey[k]] = scaVal[k];
			}
		}


		RELEASE(buffer);
	}

	AnimatedBone* FindBone(std::string name)
	{
		auto iter = std::find_if(bones.begin(), bones.end(),
			[&](const AnimatedBone& Bone)
			{
				return Bone.name == name;
			}
		);
		if (iter == bones.end()) return nullptr;
		else return &(*iter);
	}
};