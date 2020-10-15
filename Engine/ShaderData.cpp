#include "pch.h"
#include "ShaderData.h"
#include "TextureManager.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Editor;

ShaderData::ShaderData(const std::string& shaderName, const std::string& compiledShaderPath)
	: m_RawCompiledShaderPath(compiledShaderPath)
	, m_VertexCompiledShaderPath(compiledShaderPath + "-VertexShader.cso")
	, m_PixelCompiledShaderPath(compiledShaderPath + "-PixelShader.cso")
	, m_ShaderName(shaderName)
{

}

void ShaderData::AddProperty(const std::string& texturePath, ShaderType shaderType)
{
	AddProperty(StringToWString(texturePath), shaderType);
}

void ShaderData::AddProperty(const std::wstring& texturePath, ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		m_VertexShaderProperties.push_back(&TextureManager::GetInstance()->GetTexture(texturePath));
		break;
	case ShaderType::PIXEL_SHADER:
		m_PixelShaderProperties.push_back(&TextureManager::GetInstance()->GetTexture(texturePath));
		break;
	}
}

const std::string& ShaderData::GetCompiledShaderPath(ShaderType shaderType) const
{
	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		return m_VertexCompiledShaderPath;
	case ShaderType::PIXEL_SHADER:
		return m_PixelCompiledShaderPath;
	}

	assert(false); // Vertex & pixel shader are only supported atm
	return m_VertexCompiledShaderPath; // Just so the program compiles (assert will catch the issue)
}