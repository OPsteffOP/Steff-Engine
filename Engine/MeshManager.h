#pragma once
#include "MeshData.h"
#include <unordered_map>

namespace SteffEngine
{
	namespace Core
	{
		class MeshManager
		{
		public:
			PLUGIN_API static MeshManager* GetInstance();
			static void Destroy();

			PLUGIN_API MeshData* GetMeshData(const std::wstring& meshFilePath);
			PLUGIN_API MeshData* RegisterMeshData(const std::wstring& name, std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices);

		private:
			MeshManager() = default;

			static MeshManager* m_pInstance;
			std::unordered_map<std::wstring, MeshData> m_MeshData;
		};
	}
}