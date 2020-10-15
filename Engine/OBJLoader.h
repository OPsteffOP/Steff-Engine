#pragma once
#include "VertexTypes.h"
#include "Config.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace MeshLoaders
		{
			class OBJLoader
			{
			public:
				static bool LoadMesh(const std::wstring& objFilePath, std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices);

			private:
				static bool RemovePrefix(const std::string& prefix, std::string& line);
				static XMFLOAT3 GetFloat3FromLine(const std::string& line, const char delimiter = ' ');
				static XMFLOAT2 GetFloat2FromLine(const std::string& line, const char delimiter = ' ');
				static std::tuple<uint32_t, uint32_t, uint32_t> GetUInt3FromLine(const std::string& line);
				static std::wstring GetFileName(const std::wstring& filePath);
			};
		}
	}
}