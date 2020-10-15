#pragma once
#include "PixelShader.h"
#include "ConstantBufferDynamic.h"
#include "Renderer.h"

namespace SteffEngine
{
	namespace Core
	{
		class RenderTarget;

		class PostProcessingEffect : public PixelShader
		{
		public:
			PLUGIN_API explicit PostProcessingEffect(const std::wstring& compiledShaderPath);
			virtual ~PostProcessingEffect();

			PostProcessingEffect(const PostProcessingEffect& postProcessingEffect) = delete;
			PostProcessingEffect(PostProcessingEffect&& postProcessingEffect) = delete;
			PostProcessingEffect& operator=(const PostProcessingEffect& postProcessingEffect) = delete;
			PostProcessingEffect& operator=(PostProcessingEffect&& postProcessingEffect) = delete;

			void Prepare(Renderer& renderer, ID3D11ShaderResourceView* pCurrentRenderState);

		protected:
			virtual void Bind() const override;

		private:
			RenderTarget* m_pRenderTarget;
			VertexShader* m_pPostProcessingVertexShader;

			ID3D11SamplerState* m_pSamplerState;
			ID3D11BlendState* m_pBlendState;
			ID3D11DepthStencilState* m_pDepthStencilState;
		};
	}
}