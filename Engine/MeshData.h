#pragma once
#include <vector>

namespace SteffEngine
{
	namespace Core
	{
		class MeshData
		{
		public:
			explicit MeshData(std::vector<PosTexVertex>& vertices, std::vector<uint32_t>& indices);
			explicit MeshData(std::vector<PosTexVertex>&& vertices, std::vector<uint32_t>&& indices);
			MeshData(const MeshData& meshData) = delete;
			MeshData(MeshData&& meshData) = delete;

			~MeshData();

			MeshData& operator=(const MeshData& meshData) = delete;
			MeshData& operator=(MeshData&& meshData) = delete;

			inline ID3D11Buffer* GetVertexBuffer() const { return m_pVertexBuffer; };
			inline ID3D11Buffer* GetIndexBuffer() const { return m_pIndexBuffer; };

			void BindVertexBuffer() const;
			void BindIndexBuffer() const;

			const std::vector<PosTexVertex> vertices;
			const std::vector<uint32_t> indices;

		private:
			void CreateBuffers();

			ID3D11Buffer* m_pVertexBuffer;
			ID3D11Buffer* m_pIndexBuffer;
		};
	}
}