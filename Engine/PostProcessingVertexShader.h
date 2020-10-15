#pragma once
#include "VertexShader.h"

namespace SteffEngine
{
	namespace Core
	{
		class PostProcessingVertexShader : public VertexShader
		{
		public:
			PLUGIN_API explicit PostProcessingVertexShader(const std::wstring& compiledShaderPath);

			PLUGIN_API virtual void SetWorldMatrix(const XMFLOAT4X4& worldMatrix) override {};
			PLUGIN_API virtual void SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) override {};

		protected:
			virtual void Bind() const override;
		};
	}
}