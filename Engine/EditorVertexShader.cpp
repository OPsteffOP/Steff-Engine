#include "pch.h"
#include "EditorVertexShader.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

EditorVertexShader::EditorVertexShader(const std::wstring& compiledShaderPath)
	: VertexShader(compiledShaderPath)
	, m_MeshConstantBuffer(ConstantBufferStatic{ Mesh_ConstantBuffer{ IDENTITY_MATRIX } })
	, m_CameraConstantBuffer(ConstantBufferDynamic{ Camera_ConstantBuffer{ IDENTITY_MATRIX } })
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

void EditorVertexShader::Bind() const
{
	m_MeshConstantBuffer.BindVS(0);
	m_CameraConstantBuffer.BindVS(1);

	DeviceManager::m_pDeviceContext->VSSetShaderResources(0, unsigned int(m_pResourceViews.size()), m_pResourceViews.data());
}

PLUGIN_API
void EditorVertexShader::SetWorldMatrix(const XMFLOAT4X4& worldMatrix) // row major
{
	if (m_MeshConstantBuffer.data.worldMatrix == worldMatrix)
		return;

	const XMMATRIX transposedWorldMatrix{ XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix)) };
	XMStoreFloat4x4(&m_MeshConstantBuffer.data.worldMatrix, transposedWorldMatrix);
	m_MeshConstantBuffer.ApplyChanges();
}

PLUGIN_API
void EditorVertexShader::SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) // row major
{
	if (m_CameraConstantBuffer.data.viewProjectionMatrix == viewProjectionMatrix)
		return;

	const XMMATRIX transposedViewProjectionMatrix{ XMMatrixTranspose(XMLoadFloat4x4(&viewProjectionMatrix)) };
	XMStoreFloat4x4(&m_CameraConstantBuffer.data.viewProjectionMatrix, transposedViewProjectionMatrix);
	m_CameraConstantBuffer.ApplyChanges();
}

void EditorVertexShader::AddTexture(const Texture& texture)
{
	m_pResourceViews.push_back(texture.GetShaderResourceView());
}