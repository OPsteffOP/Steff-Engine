#include "pch.h"
#include "PostProcessingVertexShader.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

PLUGIN_API
PostProcessingVertexShader::PostProcessingVertexShader(const std::wstring& compiledShaderPath)
	: VertexShader(compiledShaderPath)
{
	// =-=-=-=-= CREATING THE INPUT LAYOUT =-=-=-=-= //
	const uint32_t inputLayoutElements{ 2 };
	D3D11_INPUT_ELEMENT_DESC inputLayout[inputLayoutElements]{};
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	inputLayout[1].SemanticName = "TEXCOORD";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayout[1].AlignedByteOffset = 12;
	inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	HRESULT result = DeviceManager::m_pDevice->CreateInputLayout(inputLayout, inputLayoutElements,
		m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), &m_pInputLayout);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create input layout");
	}
}

void PostProcessingVertexShader::Bind() const
{

}