#pragma once
namespace SteffEngine
{
	namespace Core
	{
		enum class RenderState
		{
			SOLID,
			WIREFRAME
		};

		class Scene;

		class MeshData;
		class Texture;

		class VertexShader;
		class GeometryShader;
		class PixelShader;

		class RenderTarget;

		class Renderer
		{
		public:
			explicit Renderer(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight);
			Renderer(const Renderer& renderer) = delete;
			Renderer(Renderer&& renderer) = delete;

			~Renderer();

			Renderer& operator=(const Renderer& renderer) = delete;
			Renderer& operator=(Renderer&& renderer) = delete;

			void SetRenderState(RenderState renderState);
			void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology);
			void SetSamplerState(ID3D11SamplerState* pSamplerState, unsigned int slot = 0);
			void SetBlendState(ID3D11BlendState* pBlendState);
			void SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState);

			void SetMesh(const MeshData* pMeshData);
			void SetVertexBuffer(ID3D11Buffer* const* pBuffers, unsigned int bufferCount, const unsigned int* pStrides, const unsigned int* pOffsets);
			void SetIndexBuffer(ID3D11Buffer* pBuffer);

			void SetVertexShader(const VertexShader* pShader);
			void SetGeometryShader(const GeometryShader* pShader);
			void SetPixelShader(const PixelShader* pShader);

			void Draw(unsigned int vertices);
			void DrawIndexed(unsigned int indices);
			void DrawIndexedInstanced(unsigned int indices, unsigned int instances);

			RenderTarget* GetCurrentRenderTarget() const;
			void SetCurrentRenderTarget(RenderTarget* pRenderTarget);

			void InitializeFrame();
			void PresentFrame();

		private:
			void InitializeDirectX(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight);

			IDXGISwapChain* m_pSwapChain;

			RenderTarget* m_pDefaultRenderTarget;
			RenderTarget* m_pCurrentRenderTarget;

			bool m_RenderStateChanged;
			ID3D11RasterizerState* m_pRasterizerState;
		};
	}
}