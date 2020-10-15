#include "pch.h"
#include "PixelShader.h"
#include "DeviceManager.h"
#include "ShaderManager.h"

using namespace SteffEngine::Core;

PLUGIN_API
PixelShader::PixelShader(const std::wstring& compiledShaderPath)
{
	const std::pair<ID3DBlob*, ID3D11PixelShader*> pShaderData{ ShaderManager::GetInstance()->GetPixelShader(compiledShaderPath) };
	m_pShaderBlob = pShaderData.first;
	m_pShader = pShaderData.second;
}

PLUGIN_API
void PixelShader::RootBind() const
{
	DeviceManager::m_pDeviceContext->PSSetShader(m_pShader, nullptr, 0);
	Bind();
}