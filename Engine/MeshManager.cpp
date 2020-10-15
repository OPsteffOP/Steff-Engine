#include "pch.h"
#include "MeshManager.h"
#include "OBJLoader.h"
#include <filesystem>

using namespace SteffEngine::Core;

MeshManager* MeshManager::m_pInstance{ nullptr };

PLUGIN_API
MeshManager* MeshManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MeshManager{};
	}

	return m_pInstance;
}

void MeshManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

PLUGIN_API
MeshData* MeshManager::GetMeshData(const std::wstring& meshFilePath)
{
	std::unordered_map<std::wstring, MeshData>::iterator foundMeshDataIterator{ m_MeshData.find(meshFilePath) };
	if (foundMeshDataIterator == m_MeshData.end())
	{
		std::vector<PosTexVertex> vertices;
		std::vector<uint32_t> indices;

		const std::filesystem::path path{ meshFilePath };
		if (path.extension().wstring() == L".obj")
		{
			const bool loadedSuccess{ MeshLoaders::OBJLoader::LoadMesh(meshFilePath, vertices, indices) };
			if (!loadedSuccess)
			{
				Logging::Log(LogType::ERROR, L"Failed to load mesh data: " + meshFilePath);
				return nullptr;
			}
		}

		foundMeshDataIterator = m_MeshData.emplace(std::piecewise_construct,
			std::forward_as_tuple(meshFilePath),
			std::forward_as_tuple(std::move(vertices), std::move(indices))).first;
	}

	return &foundMeshDataIterator->second;
}

PLUGIN_API
MeshData* MeshManager::RegisterMeshData(const std::wstring& name, std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices)
{
	std::unordered_map<std::wstring, MeshData>::iterator foundMeshDataIterator{ m_MeshData.find(name) };
	if (foundMeshDataIterator == m_MeshData.end())
	{
		foundMeshDataIterator = m_MeshData.emplace(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(std::move(vertices), std::move(indices))).first;
	}

	return &foundMeshDataIterator->second;
}