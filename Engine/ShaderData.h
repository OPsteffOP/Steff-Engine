#pragma once
#include "ShaderType.h"

namespace SteffEngine
{
	namespace Editor
	{
		class ShaderData
		{
		public:
			explicit ShaderData(const std::string& shaderName, const std::string& compiledShaderPath);

			void AddProperty(const std::string& texturePath, SteffEngine::Core::ShaderType shaderType);
			void AddProperty(const std::wstring& texturePath, SteffEngine::Core::ShaderType shaderType);

			const std::string& GetShaderName() const { return m_ShaderName; };
			const std::string& GetRawCompiledShaderPath() const { return m_RawCompiledShaderPath; };
			const std::string& GetCompiledShaderPath(SteffEngine::Core::ShaderType shaderType) const;
			const std::vector<SteffEngine::Core::Texture*>& GetVertexShaderProperties() const { return m_VertexShaderProperties; };
			const std::vector<SteffEngine::Core::Texture*>& GetPixelShaderProperties() const { return m_PixelShaderProperties; };

		private:
			const std::string m_ShaderName;
			const std::string m_RawCompiledShaderPath;
			const std::string m_VertexCompiledShaderPath;
			const std::string m_PixelCompiledShaderPath;
			std::vector<SteffEngine::Core::Texture*> m_VertexShaderProperties;
			std::vector<SteffEngine::Core::Texture*> m_PixelShaderProperties;
		};
	}
}