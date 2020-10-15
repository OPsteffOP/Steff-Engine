#pragma once
#include "PixelShader.h"
#include "Texture.h"

namespace SteffEngine
{
	namespace Core
	{
		class EditorPixelShader : public PixelShader
		{
		public:
			explicit EditorPixelShader(const std::wstring& compiledShaderPath);

			void AddTexture(const Texture& texture);

		protected:
			virtual void Bind() const override;

		private:
			std::vector<ID3D11ShaderResourceView*> m_pResourceViews;
		};
	}
}