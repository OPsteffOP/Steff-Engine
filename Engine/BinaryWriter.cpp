#include "pch.h"
#include "BinaryWriter.h"
#include <fstream>

using namespace SteffEngine::Core::MeshLoaders;

bool BinaryWriter::Write(const std::wstring& binaryFilePath, const std::vector<PosTexVertex>& vertices, const std::vector<uint32_t>& indices)
{
	Logging::Log(LogType::INFORMATION, L"Writing binary: " + binaryFilePath);

	std::ofstream output{ binaryFilePath, std::fstream::out | std::fstream::binary };
	if (!output)
	{
		Logging::Log(LogType::ERROR, L"Failed opening binary file to write: " + binaryFilePath);
		return false;
	}

	const size_t verticesCount{ vertices.size() };
	const size_t indicesCount{ indices.size() };

	// Specify that the next stream will be x amount of vertices
	WriteData(output, verticesCount);

	for (const PosTexVertex& vertex : vertices)
	{
		WriteData(output, vertex.position.x);
		WriteData(output, vertex.position.y);
		WriteData(output, vertex.position.z);

		WriteData(output, vertex.textureCoordinate.x);
		WriteData(output, vertex.textureCoordinate.y);
	}

	// Specify that the next stream will be x amount of indices
	WriteData(output, indicesCount);

	for (uint32_t index : indices)
	{
		WriteData(output, index);
	}

	return true;
}