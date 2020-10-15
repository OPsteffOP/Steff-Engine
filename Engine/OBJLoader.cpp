#include "pch.h"
#include "OBJLoader.h"
#include "BinaryWriter.h"
#include "BinaryLoader.h"
#include <fstream>
#include <string>
#include <regex>

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::MeshLoaders;

bool OBJLoader::LoadMesh(const std::wstring& objFilePath, std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices)
{
	const std::wstring configFilePath{ L"BinaryMeshesMapped.conf" };
	Config& config{ ConfigManager::GetInstance()->GetConfig(configFilePath) };

	const std::optional<std::reference_wrapper<const std::wstring>> foundBinaryFilePath{ config.GetValue(objFilePath) };
	if (foundBinaryFilePath != std::nullopt)
	{
		if (BinaryLoader::Read(foundBinaryFilePath.value(), vertices, indices))
		{
			return true;
		}

		vertices.clear();
		indices.clear();
	}

	Logging::Log(LogType::INFORMATION, L"Loading OBJ: " + objFilePath);

	std::ifstream input{ objFilePath };
	if (!input)
	{
		return false;
	}

	std::vector<XMFLOAT3> tempPositions;
	std::vector<XMFLOAT2> tempTextureCoordinates;
	std::vector<std::pair<uint32_t, uint32_t>> tempFaces;

	std::string line;
	while (std::getline(input, line))
	{
		if (RemovePrefix("v", line))
		{
			XMFLOAT3 position{ GetFloat3FromLine(line) }; // right handed coordinate system
			position.z = -position.z; // change to left handed coordinate system

			tempPositions.push_back(position);
		}
		else if (RemovePrefix("vt", line))
		{
			const XMFLOAT2 textureCoordinateFloat2{ GetFloat2FromLine(line) };
			tempTextureCoordinates.emplace_back(textureCoordinateFloat2.x, textureCoordinateFloat2.y);
		}
		else if (RemovePrefix("f", line))
		{
			const size_t spacePosition1{ line.find(' ') + 1 };
			const size_t spacePosition2{ line.find(' ', spacePosition1) + 1 };

			std::tuple<uint32_t, uint32_t, uint32_t> faceData{ GetUInt3FromLine(line) };
			tempFaces.emplace_back(std::get<0>(faceData), std::get<1>(faceData));

			faceData = GetUInt3FromLine(line.substr(spacePosition2)); // swap the last 2 face to get a clockwise order
			tempFaces.emplace_back(std::get<0>(faceData), std::get<1>(faceData));

			faceData = GetUInt3FromLine(line.substr(spacePosition1));
			tempFaces.emplace_back(std::get<0>(faceData), std::get<1>(faceData));
		}
	}
	
	for (const std::pair<uint32_t, uint32_t>& face : tempFaces)
	{
		vertices.push_back(PosTexVertex{ tempPositions[face.first], tempTextureCoordinates[face.second] });
		indices.push_back(uint32_t(vertices.size() - 1));
	}

	const std::wstring newBinaryFilePath{ BINARY_MESHES_FOLDER + GetFileName(objFilePath) + L".bSE" };
	const bool binaryWriteSuccess{ BinaryWriter::Write(newBinaryFilePath, vertices, indices) };
	if (binaryWriteSuccess)
	{
		config.SetValue(objFilePath, newBinaryFilePath);
		config.Save();
	}

	Logging::Log(LogType::INFORMATION, L"OBJ file loaded & parsed: " + objFilePath);
	return true;
}

bool OBJLoader::RemovePrefix(const std::string& prefix, std::string& line)
{
	const std::regex regex{ prefix + " +" + "(.*)" };
	if (std::regex_match(line, regex))
	{
		std::smatch matches;
		if (std::regex_search(line, matches, regex))
		{
			line = matches[1];
			return true;
		}
	}

	return false;
}

XMFLOAT3 OBJLoader::GetFloat3FromLine(const std::string& line, char delimiter)
{
	XMFLOAT3 float3{};

	size_t previousSpacePosition{};
	size_t currentSpacePosition{ line.find(delimiter) };
	float3.x = std::stof(line.substr(0, currentSpacePosition));

	previousSpacePosition = currentSpacePosition + 1;
	currentSpacePosition = line.find(delimiter, previousSpacePosition);
	float3.y = std::stof(line.substr(previousSpacePosition, currentSpacePosition));

	previousSpacePosition = currentSpacePosition + 1;
	currentSpacePosition = line.find(delimiter, previousSpacePosition);
	float3.z = std::stof(line.substr(previousSpacePosition, currentSpacePosition));

	return float3;
}

XMFLOAT2 OBJLoader::GetFloat2FromLine(const std::string& line, char delimiter)
{
	XMFLOAT2 float2;

	size_t previousSpacePosition{};
	size_t currentSpacePosition{ line.find(delimiter) };
	float2.x = std::stof(line.substr(0, currentSpacePosition));

	previousSpacePosition = currentSpacePosition + 1;
	currentSpacePosition = line.find(delimiter, previousSpacePosition);
	float2.y = std::stof(line.substr(previousSpacePosition, currentSpacePosition));

	return float2;
}

std::tuple<uint32_t, uint32_t, uint32_t> OBJLoader::GetUInt3FromLine(const std::string& line)
{
	const XMFLOAT3 float3{ GetFloat3FromLine(line, '/') };
	return std::make_tuple(uint32_t(float3.x) - 1, uint32_t(float3.y) - 1, uint32_t(float3.z) - 1);
}

std::wstring OBJLoader::GetFileName(const std::wstring& filePath)
{
	const size_t lastBackSlashPosition{ filePath.rfind('\\') + 1 };
	const size_t lastForwardSlashPosition{ filePath.rfind('/') + 1 };

	size_t startPosition{ 0 };
	if (lastBackSlashPosition != std::string::npos && lastBackSlashPosition > startPosition)
	{
		startPosition = lastBackSlashPosition;
	}
	if (lastForwardSlashPosition != std::string::npos && lastForwardSlashPosition > startPosition)
	{
		startPosition = lastForwardSlashPosition;
	}

	const size_t lastDotPosition{ filePath.rfind('.') };

	return filePath.substr(startPosition, lastDotPosition - startPosition);
}