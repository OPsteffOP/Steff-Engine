#include "pch.h"
#include "TexColPixelShader.h"
#include "TextureManager.h"

using namespace SteffEngine::Core;

PLUGIN_API
TexColPixelShader::TexColPixelShader(const std::wstring& compiledShaderPath, const std::wstring& diffuseTexturePath)
	: PixelShader(compiledShaderPath)
	, m_ColorsConstantBuffer(ConstantBufferStatic{ Colors_ConstantBuffer{ XMFLOAT4{ 0.f, 0.f, 0.f, 0.f } } })
	, m_pDiffuseTexture(&TextureManager::GetInstance()->GetTexture(diffuseTexturePath))
{

}

void TexColPixelShader::Bind() const
{
	ID3D11ShaderResourceView* pDiffuseTextureShaderResourceView{ m_pDiffuseTexture->GetShaderResourceView() };
	DeviceManager::m_pDeviceContext->PSSetShaderResources(0, 1, &pDiffuseTextureShaderResourceView);

	m_ColorsConstantBuffer.BindPS(0);
}

PLUGIN_API
void TexColPixelShader::SetColor(const XMFLOAT4& color)
{
	m_ColorsConstantBuffer.data.highlightColor = color;
	m_ColorsConstantBuffer.ApplyChanges();
}

PLUGIN_API
void TexColPixelShader::SetDiffuseTexture(const std::wstring& diffuseTexturePath)
{
	m_pDiffuseTexture = &TextureManager::GetInstance()->GetTexture(diffuseTexturePath);
}