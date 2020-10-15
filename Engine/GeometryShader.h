#pragma once
#include "Shader.h"

namespace SteffEngine
{
	namespace Core
	{
		class GeometryShader : public Shader
		{
		public:
			PLUGIN_API explicit GeometryShader(const std::wstring& compiledShaderPath);
			virtual ~GeometryShader() = default;

			PLUGIN_API virtual void RootBind() const override;

		protected:
			ID3D11GeometryShader* m_pShader;
		};
	}
}