#pragma once
#include "VertexTypes.h"
#include <fstream>

namespace SteffEngine
{
	namespace Core
	{
		namespace MeshLoaders
		{
			class BinaryWriter
			{
			public:
				static bool Write(const std::wstring& binaryFilePath, const std::vector<PosTexVertex>& vertices, const std::vector<uint32_t>& indices);

			private:
				template<typename T>
				static void WriteData(std::ofstream& output, const T& data);
			};

			template<typename T>
			void BinaryWriter::WriteData(std::ofstream& output, const T& data)
			{
				output.write((const char*) &data, sizeof(T));
			}
		}
	}
}