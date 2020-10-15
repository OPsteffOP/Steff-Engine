#pragma once
#include "ConstantBufferDynamic.h"
#include "Renderer.h"
#include <vector>

namespace SteffEngine
{
	namespace Debugging
	{
		struct VS_ConstantBuffer
		{
			XMFLOAT4X4 worldViewProjectionMatrix;
		};

		class LineDebugging
		{
		public:
			static LineDebugging* GetInstance();
			static void Destroy();

			void AddLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint);
			void AddPermanentLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint);
			
			void Update();
			void Render(SteffEngine::Core::Renderer& renderer);

		private:
			LineDebugging();
			~LineDebugging();

			void Initialize();
			void InitializeVertexBuffer();
			void InitializePermanentVertexBuffer();

			static LineDebugging* m_pInstance;
			static uint32_t m_MaxPermanentVertices;

			std::vector<XMFLOAT3> m_PermanentVertices;
			ID3D11Buffer* m_pPermanentVertexBuffer;

			uint32_t m_CurrentMaxVertices;
			std::vector<XMFLOAT3> m_Vertices;
			ID3D11Buffer* m_pVertexBuffer;

			SteffEngine::Core::VertexShader* m_pVertexShader;
			SteffEngine::Core::PixelShader* m_pPixelShader;
		};
	}
}