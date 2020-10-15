#include "pch.h"
#include "GeometryShader.h"
#include "ShaderManager.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

PLUGIN_API
GeometryShader::GeometryShader(const std::wstring& compiledShaderPath)
{
	const std::pair<ID3DBlob*, ID3D11GeometryShader*> pShaderData{ ShaderManager::GetInstance()->GetGeometryShader(compiledShaderPath) };
	m_pShaderBlob = pShaderData.first;
	m_pShader = pShaderData.second;
}

PLUGIN_API
void GeometryShader::RootBind() const
{
	DeviceManager::m_pDeviceContext->GSSetShader(m_pShader, nullptr, 0);
	Bind();
}