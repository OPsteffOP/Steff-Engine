#include "pch.h"
#include "PosTexInstanceVertexShader.h"

using namespace SteffEngine::Core;

PLUGIN_API
PosTexInstanceVertexShader::PosTexInstanceVertexShader(const std::wstring& compiledShaderPath)
	: VertexShader(compiledShaderPath)
	, m_CameraConstantBuffer(ConstantBufferDynamic{ Camera_ConstantBuffer{ IDENTITY_MATRIX } })
{
	// =-=-=-=-= CREATING THE INPUT LAYOUT =-=-=-=-= //
	const uint32_t inputLayoutElements{ 6 };
	D3D11_INPUT_ELEMENT_DESC inputLayout[inputLayoutElements]{};
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].InputSlot = 0; // using the vertex buffer
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	inputLayout[1].SemanticName = "TEXCOORD";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 12
	inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	inputLayout[2].SemanticName = "WORLD";
	inputLayout[2].SemanticIndex = 0;
	inputLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[2].InputSlot = 1; // using the instance buffer
	inputLayout[2].AlignedByteOffset = 0; // back to 0 because using another buffer
	inputLayout[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	inputLayout[2].InstanceDataStepRate = 1;

	inputLayout[3].SemanticName = "WORLD";
	inputLayout[3].SemanticIndex = 1;
	inputLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[3].InputSlot = 1;
	inputLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 16
	inputLayout[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	inputLayout[3].InstanceDataStepRate = 1;

	inputLayout[4].SemanticName = "WORLD";
	inputLayout[4].SemanticIndex = 2;
	inputLayout[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[4].InputSlot = 1;
	inputLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 32
	inputLayout[4].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	inputLayout[4].InstanceDataStepRate = 1;

	inputLayout[5].SemanticName = "WORLD";
	inputLayout[5].SemanticIndex = 3;
	inputLayout[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[5].InputSlot = 1;
	inputLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 48
	inputLayout[5].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	inputLayout[5].InstanceDataStepRate = 1;

	HRESULT result = DeviceManager::m_pDevice->CreateInputLayout(inputLayout, inputLayoutElements,
		m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), &m_pInputLayout);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create input layout");
	}
}

void PosTexInstanceVertexShader::Bind() const
{
	m_MeshConstantBuffer.BindVS(0);
	m_CameraConstantBuffer.BindVS(1);
}

PLUGIN_API
void PosTexInstanceVertexShader::SetWorldMatrix(const XMFLOAT4X4& worldMatrix) // row major
{
	if (m_MeshConstantBuffer.data.worldMatrix == worldMatrix)
		return;

	const XMMATRIX transposedWorldMatrix{ XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix)) };
	XMStoreFloat4x4(&m_MeshConstantBuffer.data.worldMatrix, transposedWorldMatrix);
	m_MeshConstantBuffer.ApplyChanges();
}

PLUGIN_API
void PosTexInstanceVertexShader::SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) // row major
{
	if (m_CameraConstantBuffer.data.viewProjectionMatrix == viewProjectionMatrix)
		return;

	const XMMATRIX transposedViewProjectionMatrix{ XMMatrixTranspose(XMLoadFloat4x4(&viewProjectionMatrix)) };
	XMStoreFloat4x4(&m_CameraConstantBuffer.data.viewProjectionMatrix, transposedViewProjectionMatrix);
	m_CameraConstantBuffer.ApplyChanges();
}