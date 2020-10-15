#include "pch.h"
#include "InstancedMeshRendererComponent.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "GameObject.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION(InstancedMeshRendererComponent);

PLUGIN_API
InstancedMeshRendererComponent::InstancedMeshRendererComponent(unsigned int maxInstances, const std::wstring& meshFilePath, VertexShader* pVertexShader, PixelShader* pPixelShader, GeometryShader* pGeometryShader, bool isUsingTransparency)
	: m_MaxInstances(maxInstances)
	, m_pMeshData(MeshManager::GetInstance()->GetMeshData(meshFilePath))
	, m_pVertexShader(pVertexShader)
	, m_pGeometryShader(pGeometryShader)
	, m_pPixelShader(pPixelShader)
	, m_IsUsingTransparency(isUsingTransparency)
	, m_pInstanceBuffer(nullptr)
	, m_pSamplerState(nullptr)
	, m_pBlendState(nullptr)
{
	m_pTransformComponents.reserve(m_MaxInstances);
	m_InstanceData.reserve(m_MaxInstances);
}

InstancedMeshRendererComponent::~InstancedMeshRendererComponent()
{
	std::for_each(m_pTransformComponents.begin(), m_pTransformComponents.end(),
		[](TransformComponent* pTransformComponent) { SAFE_DELETE(pTransformComponent); });

	SAFE_DELETE(m_pVertexShader);
	SAFE_DELETE(m_pGeometryShader);
	SAFE_DELETE(m_pPixelShader);

	SAFE_DELETE_DIRECTX(m_pInstanceBuffer);
	SAFE_DELETE_DIRECTX(m_pSamplerState);
	SAFE_DELETE_DIRECTX(m_pBlendState);
	SAFE_DELETE_DIRECTX(m_pDepthStencilState);
}

void InstancedMeshRendererComponent::Initialize()
{
	m_pVertexShader->SetWorldMatrix(m_pGameObject->GetTransform().GetWorldMatrix());

	HRESULT result;

	// =-=-=-=-= CREATING THE PS SAMPLER STATE =-=-=-=-= //
	D3D11_SAMPLER_DESC samplerDescriptor{};
	samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	result = DeviceManager::m_pDevice->CreateSamplerState(&samplerDescriptor, &m_pSamplerState);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create sampler state");
	}

	// =-=-=-=-= CREATING THE INSTANCE BUFFER =-=-=-=-= //
	D3D11_BUFFER_DESC instanceBufferDescriptor{};
	instanceBufferDescriptor.ByteWidth = unsigned int(sizeof(InstanceData)) * m_MaxInstances;
	instanceBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDescriptor.CPUAccessFlags = NULL;
	instanceBufferDescriptor.MiscFlags = NULL;
	instanceBufferDescriptor.StructureByteStride = unsigned int(sizeof(InstanceData));

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = m_InstanceData.data();

	result = DeviceManager::m_pDevice->CreateBuffer(&instanceBufferDescriptor, &initData, &m_pInstanceBuffer);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create instance buffer");
	}

	// =-=-=-=-= CREATING THE BLEND STATE =-=-=-=-= //
	D3D11_BLEND_DESC blendDescriptor{};
	if (m_IsUsingTransparency)
	{
		blendDescriptor.RenderTarget[0].BlendEnable = true;
		blendDescriptor.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDescriptor.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDescriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDescriptor.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDescriptor.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDescriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDescriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else
	{
		blendDescriptor.RenderTarget[0].BlendEnable = false;
		blendDescriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	result = DeviceManager::m_pDevice->CreateBlendState(&blendDescriptor, &m_pBlendState);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create blend state");
	}

	// =-=-=-=-= CREATING THE DEPTH-STENCIL STATE =-=-=-=-= //
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	result = DeviceManager::m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create depth stencil state");
	}
}

PLUGIN_API
void InstancedMeshRendererComponent::AddInstance(const std::wstring& diffuseTexturePath, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
{
	if (m_pTransformComponents.size() >= m_MaxInstances)
	{
		Logging::Log(LogType::WARNING, "Failed to add an instance (instanceCount >= maxInstances)");
		return;
	}
	
	m_pTransformComponents.push_back(new TransformComponent{ position, rotation, scale });
	m_InstanceData.push_back(InstanceData{ GetTransposedWorldMatrix(unsigned int(m_InstanceData.size())) });

	DeviceManager::m_pDeviceContext->UpdateSubresource(m_pInstanceBuffer, 0, nullptr, m_InstanceData.data(), 0, 0);
}

void InstancedMeshRendererComponent::Update()
{
	m_pVertexShader->SetViewProjectionMatrix(CameraManager::GetInstance()->GetActiveCamera()->GetInverseViewProjectionMatrix());
}

void InstancedMeshRendererComponent::Render(Renderer& renderer) const
{
	// =-=-=-=-= INPUT ASSEMBLER STAGE =-=-=-=-= //
	renderer.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Buffer* const pBuffers[]{ m_pMeshData->GetVertexBuffer(), m_pInstanceBuffer };
	const unsigned int strides[]{ sizeof(PosTexVertex), sizeof(InstanceData) };
	const unsigned int offsets[]{ 0, 0 };
	renderer.SetVertexBuffer(pBuffers, 2, strides, offsets);
	renderer.SetIndexBuffer(m_pMeshData->GetIndexBuffer());

	// =-=-=-=-= VERTEX SHADER STAGE =-=-=-=-= //
	renderer.SetVertexShader(m_pVertexShader);

	// =-=-=-=-= GEOMETRY SHADER STAGE =-=-=-=-= //
	renderer.SetGeometryShader(m_pGeometryShader);

	// =-=-=-=-= PIXEL SHADER STAGE =-=-=-=-= //
	renderer.SetPixelShader(m_pPixelShader);
	renderer.SetSamplerState(m_pSamplerState);

	// =-=-=-=-= OUTPUT MERGER STAGE =-=-=-=-= //
	renderer.SetBlendState(m_pBlendState);
	renderer.SetDepthStencilState(m_pDepthStencilState);

	// =-=-=-=-= DRAWING =-=-=-=-= //
	renderer.DrawIndexedInstanced(unsigned int(m_pMeshData->indices.size()), unsigned int(m_InstanceData.size()));
}

XMFLOAT4X4 InstancedMeshRendererComponent::GetTransposedWorldMatrix(unsigned int index) const
{
	const XMMATRIX instanceWorldMatrix{ XMLoadFloat4x4(&m_pTransformComponents[index]->GetWorldMatrix()) };

	XMFLOAT4X4 transposedInstanceWorldMatrix{};
	XMStoreFloat4x4(&transposedInstanceWorldMatrix, XMMatrixTranspose(instanceWorldMatrix));

	return transposedInstanceWorldMatrix;
}