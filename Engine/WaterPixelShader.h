#pragma once
#include "PixelShader.h"
#include "ConstantBufferStatic.h"

namespace SteffEngine
{
	namespace Core
	{
		class Texture;

		class WaterPixelShader : public PixelShader
		{
			struct Data_ConstantBuffer
			{
				XMFLOAT2 splashUVSize;
				XMFLOAT2 splashUVLeftTop;
			};

		public:
			PLUGIN_API explicit WaterPixelShader(const std::wstring& compiledShaderPath, const std::wstring& diffuseTexturePath, const std::wstring& splashTexturePath);

			PLUGIN_API void SetSplashUVSize(const XMFLOAT2& splashUVSize);
			PLUGIN_API void SetSplashUVLeftTop(const XMFLOAT2& splashUVLeftTop);
			
		protected:
			virtual void Bind() const override;

		private:
			const Texture* m_pDiffuseTexture;
			const Texture* m_pSplashTexture;

			ConstantBufferStatic<Data_ConstantBuffer> m_DataConstantBuffer;
		};
	}
}