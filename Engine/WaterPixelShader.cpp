#include "pch.h"
#include "WaterPixelShader.h"
#include "TextureManager.h"

using namespace SteffEngine::Core;

PLUGIN_API
WaterPixelShader::WaterPixelShader(const std::wstring& compiledShaderPath, const std::wstring& diffuseTexturePath, const std::wstring& splashTexturePath)
	: PixelShader(compiledShaderPath)
	, m_DataConstantBuffer(ConstantBufferStatic{ Data_ConstantBuffer{ XMFLOAT2{ -1.f, -1.f }, XMFLOAT2{ -1.f, -1.f } } })
	, m_pDiffuseTexture(&TextureManager::GetInstance()->GetTexture(diffuseTexturePath))
	, m_pSplashTexture(&TextureManager::GetInstance()->GetTexture(splashTexturePath))
{

}

void WaterPixelShader::Bind() const
{
	ID3D11ShaderResourceView* pDiffuseTextureShaderResourceView{ m_pDiffuseTexture->GetShaderResourceView() };
	DeviceManager::m_pDeviceContext->PSSetShaderResources(0, 1, &pDiffuseTextureShaderResourceView);

	ID3D11ShaderResourceView* pSplashTextureShaderResourceView{ m_pSplashTexture->GetShaderResourceView() };
	DeviceManager::m_pDeviceContext->PSSetShaderResources(1, 1, &pSplashTextureShaderResourceView);

	m_DataConstantBuffer.BindPS(0);
}

PLUGIN_API
void WaterPixelShader::SetSplashUVSize(const XMFLOAT2& splashUVSize)
{
	m_DataConstantBuffer.data.splashUVSize = splashUVSize;
	m_DataConstantBuffer.ApplyChanges();
}

PLUGIN_API
void WaterPixelShader::SetSplashUVLeftTop(const XMFLOAT2& splashUVLeftTop)
{
	m_DataConstantBuffer.data.splashUVLeftTop = splashUVLeftTop;
	m_DataConstantBuffer.ApplyChanges();
}