#include "pch.h"
#include "VertexShader.h"
#include "DeviceManager.h"
#include "ShaderManager.h"

using namespace SteffEngine::Core;

PLUGIN_API
VertexShader::VertexShader(const std::wstring& compiledShaderPath)
	: m_pInputLayout(nullptr)
	, m_RequiresTime(false)
{
	const std::pair<ID3DBlob*, ID3D11VertexShader*> pShaderData{ ShaderManager::GetInstance()->GetVertexShader(compiledShaderPath) };
	m_pShaderBlob = pShaderData.first;
	m_pShader = pShaderData.second;
}

VertexShader::~VertexShader()
{
	SAFE_DELETE_DIRECTX(m_pInputLayout);
}

PLUGIN_API
void VertexShader::RootBind() const
{
	DeviceManager::m_pDeviceContext->VSSetShader(m_pShader, nullptr, 0);
	DeviceManager::m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	Bind();
}