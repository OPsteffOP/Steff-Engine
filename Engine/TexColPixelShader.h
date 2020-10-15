#pragma once
#include "PixelShader.h"
#include "ConstantBufferStatic.h"

namespace SteffEngine
{
	namespace Core
	{
		class Texture;

		class TexColPixelShader : public PixelShader
		{
			struct Colors_ConstantBuffer
			{
				XMFLOAT4 highlightColor;
			};

		public:
			PLUGIN_API explicit TexColPixelShader(const std::wstring& compiledShaderPath, const std::wstring& diffuseTexturePath);

			PLUGIN_API virtual void SetColor(const XMFLOAT4& color);
			PLUGIN_API void SetDiffuseTexture(const std::wstring& diffuseTexturePath);

		protected:
			virtual void Bind() const override;

		private:
			const Texture* m_pDiffuseTexture;

			ConstantBufferStatic<Colors_ConstantBuffer> m_ColorsConstantBuffer;
		};
	}
}