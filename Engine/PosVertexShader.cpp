#include "pch.h"
#include "PosVertexShader.h"

using namespace SteffEngine::Core;

PLUGIN_API
PosVertexShader::PosVertexShader(const std::wstring& compiledShaderPath)
	: VertexShader(compiledShaderPath)
	, m_MeshConstantBuffer(ConstantBufferStatic{ Mesh_ConstantBuffer{ IDENTITY_MATRIX } })
	, m_CameraConstantBuffer(ConstantBufferDynamic{ Camera_ConstantBuffer{ IDENTITY_MATRIX } })
{
	// =-=-=-=-= CREATING THE INPUT LAYOUT =-=-=-=-= //
	const uint32_t inputLayoutElements{ 1 };
	D3D11_INPUT_ELEMENT_DESC inputLayout[inputLayoutElements]{};
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	HRESULT result = DeviceManager::m_pDevice->CreateInputLayout(inputLayout, inputLayoutElements,
		m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), &m_pInputLayout);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create input layout");
	}
}

void PosVertexShader::Bind() const
{
	m_MeshConstantBuffer.BindVS(0);
	m_CameraConstantBuffer.BindVS(1);
}

PLUGIN_API
void PosVertexShader::SetWorldMatrix(const XMFLOAT4X4& worldMatrix) // row major
{
	if (m_MeshConstantBuffer.data.worldMatrix == worldMatrix)
		return;

	const XMMATRIX transposedWorldMatrix{ XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix)) };
	XMStoreFloat4x4(&m_MeshConstantBuffer.data.worldMatrix, transposedWorldMatrix);
	m_MeshConstantBuffer.ApplyChanges();
}

PLUGIN_API
void PosVertexShader::SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) // row major
{
	if (m_CameraConstantBuffer.data.viewProjectionMatrix == viewProjectionMatrix)
		return;

	const XMMATRIX transposedViewProjectionMatrix{ XMMatrixTranspose(XMLoadFloat4x4(&viewProjectionMatrix)) };
	XMStoreFloat4x4(&m_CameraConstantBuffer.data.viewProjectionMatrix, transposedViewProjectionMatrix);
	m_CameraConstantBuffer.ApplyChanges();
}