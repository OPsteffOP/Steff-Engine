#include "pch.h"
#include "ColPixelShader.h"

using namespace SteffEngine::Core;

PLUGIN_API
ColPixelShader::ColPixelShader(const std::wstring& compiledShaderPath)
	: PixelShader(compiledShaderPath)
	, m_ColorsConstantBuffer(ConstantBufferStatic{ Colors_ConstantBuffer{ XMFLOAT4{ 0.f, 0.f, 0.f, 0.f } } })
{

}

void ColPixelShader::Bind() const
{
	m_ColorsConstantBuffer.BindPS(0);
}

PLUGIN_API
void ColPixelShader::SetColor(const XMFLOAT4& color)
{
	m_ColorsConstantBuffer.data.highlightColor = color;
	m_ColorsConstantBuffer.ApplyChanges();
}