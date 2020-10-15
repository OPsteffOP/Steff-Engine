#include "pch.h"
#include "MeshRendererComponent.h"
#include "MeshManager.h"
#include "DeviceManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "FrameTimer.h"
#include "GameObject.h"
#include "EditorShaderStorage.h"
#include "EditorPixelShader.h"
#include "EditorVertexShader.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Physics::Raycasting;
using namespace SteffEngine::Timers;
using namespace SteffEngine::Editor;

COMPONENT_DEFINITION(MeshRendererComponent);

PLUGIN_API
MeshRendererComponent::MeshRendererComponent(MeshData* pMeshData, VertexShader* pVertexShader, PixelShader* pPixelShader, GeometryShader* pGeometryShader, bool isUsingTransparency)
	: m_pMeshData(pMeshData)
	, m_pVertexShader(pVertexShader)
	, m_pGeometryShader(pGeometryShader)
	, m_pPixelShader(pPixelShader)
	, m_IsUsingTransparency(isUsingTransparency)
	, m_pSamplerState(nullptr)
	, m_pBlendState(nullptr)
	, m_pDepthStencilState(nullptr)

	, m_MeshFilePath("")
{

}

PLUGIN_API
MeshRendererComponent::MeshRendererComponent(const std::wstring& meshFilePath, VertexShader* pVertexShader, PixelShader* pPixelShader, GeometryShader* pGeometryShader, bool isUsingTransparency)
	: MeshRendererComponent(MeshManager::GetInstance()->GetMeshData(meshFilePath), pVertexShader, pPixelShader, pGeometryShader, isUsingTransparency)
{

}

/* DEFAULT CONSTRUCTOR FOR EDITOR */
#include "PosTexVertexShader.h" // TODO: REMOVE - TEMP FOR TESTING
MeshRendererComponent::MeshRendererComponent()
	: m_pMeshData(nullptr)
	, m_Shader(0)
	, m_IsUsingTransparency(false)

	, m_MeshFilePath("")
{
	UpdateShaders();
}
/* DEFAULT CONSTRUCTOR FOR EDITOR */

MeshRendererComponent::~MeshRendererComponent()
{
	SAFE_DELETE(m_pVertexShader);
	SAFE_DELETE(m_pGeometryShader);
	SAFE_DELETE(m_pPixelShader);

	SAFE_DELETE_DIRECTX(m_pSamplerState);
	SAFE_DELETE_DIRECTX(m_pBlendState);
	SAFE_DELETE_DIRECTX(m_pDepthStencilState);
}

void MeshRendererComponent::Initialize()
{
	if (m_pVertexShader != nullptr)
		m_pVertexShader->SetWorldMatrix(m_pGameObject->GetTransform().GetWorldMatrix());

	HRESULT result;

	// =-=-=-=-= CREATING THE SAMPLER STATE =-=-=-=-= //
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

void MeshRendererComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_MeshFilePath")
	{
		m_pMeshData = MeshManager::GetInstance()->GetMeshData(StringToWString(m_MeshFilePath));
	}
	else if (variableName == "m_Shader")
	{
		UpdateShaders();
	}
	else if (variableName == "m_IsUsingTransparency")
	{
		SAFE_DELETE_DIRECTX(m_pBlendState);

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

		HRESULT result = DeviceManager::m_pDevice->CreateBlendState(&blendDescriptor, &m_pBlendState);
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create blend state");
		}
	}
}

void MeshRendererComponent::Update()
{
	if (m_pVertexShader == nullptr)
		return;

	m_pVertexShader->SetWorldMatrix(m_pGameObject->GetTransform().GetWorldMatrix()); // TODO: ONLY DO THE UPDATE IF IT'S A NON-STATIC OBJECT?
	m_pVertexShader->SetViewProjectionMatrix(CameraManager::GetInstance()->GetActiveCamera()->GetInverseViewProjectionMatrix());
}

void MeshRendererComponent::Render(Renderer& renderer) const
{
	if (m_pMeshData == nullptr || m_pVertexShader == nullptr || m_pPixelShader == nullptr)
		return;

	// =-=-=-=-= INPUT ASSEMBLER STAGE =-=-=-=-= //
	renderer.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderer.SetMesh(m_pMeshData);

	// =-=-=-=-= VERTEX SHADER STAGE =-=-=-=-= //
	renderer.SetVertexShader(m_pVertexShader);

	// =-=-=-=-= GEOMETRY SHADER STAGE =-=-=-=-= //
	renderer.SetGeometryShader(m_pGeometryShader);

	// =-=-=-=-= PIXEL SHADER STAGE =-=-=-=-= //
	renderer.SetPixelShader(m_pPixelShader);
	renderer.SetSamplerState(m_pSamplerState, 0);

	// =-=-=-=-= OUTPUT MERGER STAGE =-=-=-=-= //
	renderer.SetBlendState(m_pBlendState);
	renderer.SetDepthStencilState(m_pDepthStencilState);

	// =-=-=-=-= DRAWING =-=-=-=-= //
	renderer.DrawIndexed(unsigned int(m_pMeshData->indices.size()));
}

PLUGIN_API
void MeshRendererComponent::SetHighlightColor(const XMFLOAT4& highlightColor)
{
	//m_pPixelShader->SetColor(highlightColor);
}

bool MeshRendererComponent::RayHit(const Ray& ray, HitRecord& hitRecord) const
{
	for (size_t i{ 0 }; i < m_pMeshData->indices.size(); i += 3)
	{
		const PosTexVertex& v0{ m_pMeshData->vertices[m_pMeshData->indices[i]] };
		const PosTexVertex& v1{ m_pMeshData->vertices[m_pMeshData->indices[i + 1]] };
		const PosTexVertex& v2{ m_pMeshData->vertices[m_pMeshData->indices[i + 2]] };

		if (Raycasting::TriangleIntersection(hitRecord, ray, v0, v1, v2))
		{
			return true;
		}
	}

	return false;
}

PLUGIN_API
std::pair<XMFLOAT3, XMFLOAT3> MeshRendererComponent::GetCollisionBox() const
{
	XMFLOAT3 bottomLeft{};
	XMFLOAT3 topRight{};

	bottomLeft.x = std::min_element(m_pMeshData->vertices.cbegin(), m_pMeshData->vertices.cend(),
		[](const PosTexVertex& vertex1, const PosTexVertex& vertex2) { return vertex1.position.x < vertex2.position.x; })->position.x;
	bottomLeft.y = std::min_element(m_pMeshData->vertices.cbegin(), m_pMeshData->vertices.cend(),
		[](const PosTexVertex& vertex1, const PosTexVertex& vertex2) { return vertex1.position.y < vertex2.position.y; })->position.y;
	bottomLeft.z = std::min_element(m_pMeshData->vertices.cbegin(), m_pMeshData->vertices.cend(),
		[](const PosTexVertex& vertex1, const PosTexVertex& vertex2) { return vertex1.position.z < vertex2.position.z; })->position.z;

	topRight.x = std::max_element(m_pMeshData->vertices.cbegin(), m_pMeshData->vertices.cend(),
		[](const PosTexVertex& vertex1, const PosTexVertex& vertex2) { return vertex1.position.x < vertex2.position.x; })->position.x;
	topRight.y = std::max_element(m_pMeshData->vertices.cbegin(), m_pMeshData->vertices.cend(),
		[](const PosTexVertex& vertex1, const PosTexVertex& vertex2) { return vertex1.position.y < vertex2.position.y; })->position.y;
	topRight.z = std::max_element(m_pMeshData->vertices.cbegin(), m_pMeshData->vertices.cend(),
		[](const PosTexVertex& vertex1, const PosTexVertex& vertex2) { return vertex1.position.z < vertex2.position.z; })->position.z;

	return std::make_pair(bottomLeft, topRight);
}

void MeshRendererComponent::UpdateShaders()
{
	const ShaderData& shaderData{ SteffEngine::Editor::EditorShaderStorage::GetShaderData(m_Shader) };

	SAFE_DELETE(m_pVertexShader);
	SAFE_DELETE(m_pPixelShader);

	m_pVertexShader = new EditorVertexShader{ StringToWString(shaderData.GetCompiledShaderPath(ShaderType::VERTEX_SHADER)) };
	for (Texture* pTexture : shaderData.GetVertexShaderProperties())
	{
		static_cast<EditorVertexShader*>(m_pVertexShader)->AddTexture(*pTexture);
	}

	m_pPixelShader = new EditorPixelShader{ StringToWString(shaderData.GetCompiledShaderPath(ShaderType::PIXEL_SHADER)) };
	for (Texture* pTexture : shaderData.GetPixelShaderProperties())
	{
		static_cast<EditorPixelShader*>(m_pPixelShader)->AddTexture(*pTexture);
	}
}