#pragma once
#include "Shader.h"

namespace SteffEngine
{
	namespace Core
	{
		class PixelShader : public Shader
		{
		public:
			PLUGIN_API explicit PixelShader(const std::wstring& compiledShaderPath);
			virtual ~PixelShader() = default;

			PLUGIN_API virtual void RootBind() const override;

		protected:
			ID3D11PixelShader* m_pShader;
		};
	}
}