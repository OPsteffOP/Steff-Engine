#pragma once
namespace SteffEngine
{
	namespace Core
	{
		struct RenderTargetDesc
		{
			struct ColorDesc
			{
				DXGI_FORMAT format;
				bool bindableShaderResource;
			};

			struct DepthDesc
			{
				DXGI_FORMAT format;
				bool bindableShaderResource;
			};

			ColorDesc color;
			DepthDesc depth;
		};

		class RenderTarget
		{
		public:
			RenderTarget() = default;
			explicit RenderTarget(ID3D11Texture2D* pBuffer);
			~RenderTarget();

			RenderTarget(const RenderTarget& renderTarget) = delete;
			RenderTarget(RenderTarget&& renderTarget) = delete;
			RenderTarget& operator=(const RenderTarget& renderTarget) = delete;
			RenderTarget& operator=(RenderTarget&& renderTarget) = delete;

			void Create(RenderTargetDesc& desc);

			ID3D11Texture2D* GetColorBuffer() const;
			ID3D11RenderTargetView* GetRenderTargetView() const;
			ID3D11ShaderResourceView* GetColorShaderResourceView() const;
			ID3D11Texture2D* GetDepthStencilBuffer() const;
			ID3D11DepthStencilView* GetDepthStencilView() const;
			ID3D11ShaderResourceView* GetDepthShaderResourceView() const;

		private:
			DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT initFormat);
			DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT initFormat);

			ID3D11Texture2D* m_pRenderTargetBuffer;
			ID3D11RenderTargetView* m_pRenderTargetView;
			ID3D11ShaderResourceView* m_pColorShaderResourceView;

			ID3D11Texture2D* m_pDepthStencilBuffer;
			ID3D11DepthStencilView* m_pDepthStencilView;
			ID3D11ShaderResourceView* m_pDepthShaderResourceView;
		};
	}
}