#pragma once
#include "ShaderData.h"
#include "ShaderType.h"

namespace SteffEngine
{
	namespace Editor
	{
		class EditorShaderStorage
		{
		public:
			static void Initialize();

			static unsigned int LoadShader(const std::string& shaderName);

			static const ShaderData& GetShaderData(const std::string& shaderName);
			static const ShaderData& GetShaderData(unsigned int id);

			static unsigned int AddShaderData(ShaderData&& shaderData);
			static unsigned int AddShaderData(const std::string& shaderName, const std::string& compiledShaderPath);
			static unsigned int AddShaderData(const std::string& shaderName, const std::string& compiledShaderPath, const std::string& value, SteffEngine::Core::ShaderType shaderType);
			static unsigned int AddShaderData(const std::string& shaderName, const std::string& compiledShaderPath, const std::wstring& value, SteffEngine::Core::ShaderType shaderType);

			static void AddProperty(unsigned int id, const std::string& value, SteffEngine::Core::ShaderType shaderType);
			static void AddProperty(unsigned int id, const std::wstring& value, SteffEngine::Core::ShaderType shaderType);

			static std::pair<const char*, int> CompileShader(const char* shaderPath, SteffEngine::Core::ShaderType type);

		private:
			static unsigned int LoadShader(const std::string& shaderName, std::optional<unsigned int> shaderID);

			static unsigned int m_NextShaderID;
			static std::unordered_map<unsigned int, ShaderData> m_ShaderData;
		};
	}
}