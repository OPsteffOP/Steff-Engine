#include "pch.h"
#include "BinaryLoader.h"

using namespace SteffEngine::Core::MeshLoaders;

bool BinaryLoader::Read(const std::wstring& binaryFilePath, std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices)
{
	Logging::Log(LogType::INFORMATION, L"Loading Binary: " + binaryFilePath);

	std::ifstream input{ binaryFilePath, std::fstream::in | std::fstream::binary };
	if (!input)
	{
		Logging::Log(LogType::INFORMATION, L"Failed opening binary file to read: " + binaryFilePath);
		return false;
	}

	const size_t verticesAmount{ ReadData<size_t>(input) };
	for (size_t i{ 0 }; i < verticesAmount; ++i)
	{
		const float positionX{ ReadData<float>(input) };
		const float positionY{ ReadData<float>(input) };
		const float positionZ{ ReadData<float>(input) };

		const float textureCoordinateX{ ReadData<float>(input) };
		const float textureCoordinateY{ ReadData<float>(input) };

		vertices.push_back(PosTexVertex{ XMFLOAT3{ positionX, positionY, positionZ }, XMFLOAT2{ textureCoordinateX, textureCoordinateY } });
	}

	const size_t indicesAmount{ ReadData<size_t>(input) };
	for (size_t i{ 0 }; i < indicesAmount; ++i)
	{
		indices.push_back(ReadData<uint32_t>(input));
	}

	Logging::Log(LogType::INFORMATION, L"Binary file loaded & parsed: " + binaryFilePath);
	return true;
}