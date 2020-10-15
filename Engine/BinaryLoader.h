#pragma once
#include "VertexTypes.h"
#include <fstream>

namespace SteffEngine
{
	namespace Core
	{
		namespace MeshLoaders
		{
			class BinaryLoader
			{
			public:
				static bool Read(const std::wstring& binaryFilePath, std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices);

			private:
				template<typename T>
				static T ReadData(std::ifstream& input);
			};

			template<typename T>
			T BinaryLoader::ReadData(std::ifstream& input)
			{
				T data{};
				input.read((char*) &data, sizeof(T));
				return data;
			}
		}
	}
}