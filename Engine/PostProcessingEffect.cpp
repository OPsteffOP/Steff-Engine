#include "pch.h"
#include "PostProcessingEffect.h"
#include "RenderTarget.h"
#include "PostProcessingVertexShader.h"

using namespace SteffEngine::Core;

PLUGIN_API
PostProcessingEffect::PostProcessingEffect(const std::wstring& compiledShaderPath)
	: PixelShader(compiledShaderPath)
	, m_pRenderTarget(new RenderTarget{})
	, m_pPostProcessingVertexShader(new PostProcessingVertexShader{ L"C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Engine\\PostProcessingVertexShader.cso" })
{
	// =-=-=-=-= CREATING THE RENDER TARGET =-=-=-=-= //
	RenderTargetDesc desc{};
	desc.color.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.color.bindableShaderResource = true;
	desc.depth.format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.depth.bindableShaderResource = true;
	m_pRenderTarget->Create(desc);

	// =-=-=-=-= CREATING THE SAMPLER STATE =-=-=-=-= //
	D3D11_SAMPLER_DESC samplerDescriptor{};
	samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	HRESULT result = DeviceManager::m_pDevice->CreateSamplerState(&samplerDescriptor, &m_pSamplerState);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create sampler state");
	}

	// =-=-=-=-= CREATING THE BLEND STATE =-=-=-=-= //
	D3D11_BLEND_DESC blendDescriptor{};
	blendDescriptor.RenderTarget[0].BlendEnable = false;
	blendDescriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = DeviceManager::m_pDevice->CreateBlendState(&blendDescriptor, &m_pBlendState);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create blend state");
	}

	// =-=-=-=-= CREATING THE DEPTH-STENCIL STATE =-=-=-=-= //
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	result = DeviceManager::m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create depth stencil state");
	}
}

PostProcessingEffect::~PostProcessingEffect()
{
	SAFE_DELETE_DIRECTX(m_pSamplerState);
	SAFE_DELETE_DIRECTX(m_pBlendState);
	SAFE_DELETE_DIRECTX(m_pDepthStencilState);
	SAFE_DELETE(m_pPostProcessingVertexShader);
	SAFE_DELETE(m_pRenderTarget);
}

void PostProcessingEffect::Prepare(Renderer& renderer, ID3D11ShaderResourceView* pCurrentRenderState)
{
	renderer.SetCurrentRenderTarget(m_pRenderTarget);

	renderer.SetSamplerState(m_pSamplerState, 0);
	renderer.SetBlendState(m_pBlendState);
	renderer.SetDepthStencilState(m_pDepthStencilState);

	DeviceManager::m_pDeviceContext->PSSetShaderResources(0, 1, &pCurrentRenderState);
}

void PostProcessingEffect::Bind() const
{
	m_pPostProcessingVertexShader->RootBind();
}