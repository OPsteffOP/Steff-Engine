#pragma once
#include "PixelShader.h"
#include "ConstantBufferStatic.h"

namespace SteffEngine
{
	namespace Core
	{
		class ColPixelShader : public PixelShader
		{
			struct Colors_ConstantBuffer
			{
				XMFLOAT4 highlightColor;
			};

		public:
			PLUGIN_API explicit ColPixelShader(const std::wstring& compiledShaderPath);

			PLUGIN_API virtual void SetColor(const XMFLOAT4& color);

		protected:
			virtual void Bind() const override;

		private:
			ConstantBufferStatic<Colors_ConstantBuffer> m_ColorsConstantBuffer;
		};
	}
}