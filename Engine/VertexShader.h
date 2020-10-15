#pragma once
#include "Shader.h"

namespace SteffEngine
{
	namespace Core
	{
		class VertexShader : public Shader
		{
		public:
			PLUGIN_API explicit VertexShader(const std::wstring& compiledShaderPath);
			virtual ~VertexShader() override;

			PLUGIN_API virtual void RootBind() const override;

			PLUGIN_API virtual void SetWorldMatrix(const XMFLOAT4X4& worldMatrix) = 0;
			PLUGIN_API virtual void SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) = 0;

		protected:
			ID3D11VertexShader* m_pShader;
			ID3D11InputLayout* m_pInputLayout;

			bool m_RequiresTime		: 1;
		};
	}
}