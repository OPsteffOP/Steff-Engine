#include "pch.h"
#include "EditorPixelShader.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

EditorPixelShader::EditorPixelShader(const std::wstring& compiledShaderPath)
	: PixelShader(compiledShaderPath)
{

}

void EditorPixelShader::AddTexture(const Texture& texture)
{
	m_pResourceViews.push_back(texture.GetShaderResourceView());
}

void EditorPixelShader::Bind() const
{
	DeviceManager::m_pDeviceContext->PSSetShaderResources(0, unsigned int(m_pResourceViews.size()), m_pResourceViews.data());
}