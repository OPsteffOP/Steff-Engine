#include "pch.h"
#include "Renderer.h"
#include "GameManager.h"
#include "Scene.h"
#include "DeviceManager.h"
#include "ShaderManager.h"
#include "MeshData.h"
#include "TextureManager.h"
#include "RenderTarget.h"

using namespace SteffEngine::Core;

Renderer::Renderer(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight)
	: m_RenderStateChanged(false)
{
	InitializeDirectX(windowHandle, windowWidth, windowHeight);
}

Renderer::~Renderer()
{
	SAFE_DELETE_DIRECTX(m_pRasterizerState);
	SAFE_DELETE_DIRECTX(m_pSwapChain);

	SAFE_DELETE(m_pDefaultRenderTarget);
}

void Renderer::InitializeDirectX(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight)
{
	Logging::Log(LogType::INFORMATION, "Started DirectX initialization...");

	HRESULT result;

	// =-=-=-=-= CREATING SWAP CHAIN =-=-=-=-= //
	// https://bell0bytes.eu/the-swap-chain/
	DXGI_SWAP_CHAIN_DESC swapChainDescriptor{};
	swapChainDescriptor.BufferDesc.Width = windowWidth; // resolution width
	swapChainDescriptor.BufferDesc.Height = windowHeight;  // resolution height
	swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 1; // refresh rate in hertz numerator
	swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 60; // refresh rate in hertz denominator
	swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // unsigned-normalized-integer format that supports 8 bits per channel (including alpha) - https://www.khronos.org/opengl/wiki/Normalized_Integer#Unsigned
	swapChainDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // "Flags indicating how an image is stretched to fit a given monitor's resolution."

	// default sampler mode with no anti-aliasing - https://en.wikipedia.org/wiki/Multisample_anti-aliasing
	swapChainDescriptor.SampleDesc.Count = 1;
	swapChainDescriptor.SampleDesc.Quality = 0;

	swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // "use the back buffer as an output render target." and also make it bindable to a shader
	swapChainDescriptor.BufferCount = 2; // minimum of 2 is needed because of DXGI_SWAP_EFFECT_FLIP_DISCARD - https://devblogs.microsoft.com/directx/dxgi-flip-model/

	swapChainDescriptor.Windowed = true;
	swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // https://devblogs.microsoft.com/directx/dxgi-flip-model/
	swapChainDescriptor.Flags = NULL;

	// get the handle for the backbuffer of SDL
	swapChainDescriptor.OutputWindow = windowHandle;

	// Getting the DXGI factory from the device
	IDXGIDevice* pDxgiDevice;
	IDXGIAdapter* pDxgiAdapter;
	IDXGIFactory* pDxgiFactory;

	result = DeviceManager::m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDxgiDevice));
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to get the IDXGIDevice from D3D11 device!");
		return;
	}

	result = pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDxgiAdapter));
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to get the IDXGIAdapter from IDXGIDevice!");
		return;
	}

	result = pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pDxgiFactory));
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to get the IDXGIFactory from IDXGIAdapter!");
		return;
	}

	// Creating swap chain
	result = pDxgiFactory->CreateSwapChain(DeviceManager::m_pDevice, &swapChainDescriptor, &m_pSwapChain);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create d3d11 swapchain");
		return;
	}

	pDxgiDevice->Release();
	pDxgiAdapter->Release();
	pDxgiFactory->Release();

	// =-=-=-=-= CREATING RENDER TARGETS (BACKBUFFER) =-=-=-=-= //
	ID3D11Texture2D* pBackBuffer;
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to get the back buffer");
		return;
	}
	m_pDefaultRenderTarget = new RenderTarget{ pBackBuffer };

	RenderTargetDesc defaultRenderTargetDesc{};
	defaultRenderTargetDesc.color.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	defaultRenderTargetDesc.color.bindableShaderResource = true;
	defaultRenderTargetDesc.depth.format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	defaultRenderTargetDesc.depth.bindableShaderResource = true;
	m_pDefaultRenderTarget->Create(defaultRenderTargetDesc);

	m_pCurrentRenderTarget = m_pDefaultRenderTarget;


	// binds the back buffer render target and the depth-stencil buffer to the output-merger stage
	ID3D11RenderTargetView* pRenderTargetView{ m_pCurrentRenderTarget->GetRenderTargetView() };
	DeviceManager::m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, m_pCurrentRenderTarget->GetDepthStencilView());

	// =-=-=-=-= CREATING THE VIEWPORT =-=-=-=-= //
	D3D11_VIEWPORT viewPort{};
	viewPort.Width = static_cast<float>(windowWidth);
	viewPort.Height = static_cast<float>(windowHeight);
	viewPort.TopLeftX = 0.f;
	viewPort.TopLeftY = 0.f;
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;

	// binds the viewport to the rasterization stage
	DeviceManager::m_pDeviceContext->RSSetViewports(1, &viewPort);

	Logging::Log(LogType::INFORMATION, "DirectX initialization completed");
}

void Renderer::SetRenderState(RenderState renderState)
{
	SAFE_DELETE_DIRECTX(m_pRasterizerState);

	D3D11_RASTERIZER_DESC rasterizerDescriptor{};
	switch (renderState)
	{
	case RenderState::SOLID:
		rasterizerDescriptor.FillMode = D3D11_FILL_SOLID;
		rasterizerDescriptor.CullMode = D3D11_CULL_BACK;
		break;
	case RenderState::WIREFRAME:
		rasterizerDescriptor.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDescriptor.CullMode = D3D11_CULL_NONE;
		break;
	}
	rasterizerDescriptor.DepthClipEnable = true;

	HRESULT result{ DeviceManager::m_pDevice->CreateRasterizerState(&rasterizerDescriptor, &m_pRasterizerState) };
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create the rasterizer state");
		return;
	}

	m_RenderStateChanged = true;
}

void Renderer::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	DeviceManager::m_pDeviceContext->IASetPrimitiveTopology(primitiveTopology);
}

void Renderer::SetSamplerState(ID3D11SamplerState* pSamplerState, unsigned int slot)
{
	DeviceManager::m_pDeviceContext->VSSetSamplers(slot, 1, &pSamplerState);
	DeviceManager::m_pDeviceContext->PSSetSamplers(slot, 1, &pSamplerState);
}

void Renderer::SetBlendState(ID3D11BlendState* pBlendState)
{
	DeviceManager::m_pDeviceContext->OMSetBlendState(pBlendState, nullptr, 0xFFFFFFFF);
}

void Renderer::SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState)
{
	DeviceManager::m_pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);
}

void Renderer::SetVertexBuffer(ID3D11Buffer* const* pBuffers, unsigned int bufferCount, const unsigned int* pStrides, const unsigned int* pOffsets)
{
	DeviceManager::m_pDeviceContext->IASetVertexBuffers(0, bufferCount, pBuffers, pStrides, pOffsets);
}

void Renderer::SetIndexBuffer(ID3D11Buffer* pBuffer)
{
	DeviceManager::m_pDeviceContext->IASetIndexBuffer(pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void Renderer::SetMesh(const MeshData* pMeshData)
{
	pMeshData->BindVertexBuffer();
	pMeshData->BindIndexBuffer();
}

void Renderer::SetVertexShader(const VertexShader* pShader)
{
	pShader->RootBind();
}

void Renderer::SetGeometryShader(const GeometryShader* pShader)
{
	if (pShader == nullptr)
		return;

	pShader->RootBind();
}

void Renderer::SetPixelShader(const PixelShader* pShader)
{
	pShader->RootBind();
}

void Renderer::Draw(unsigned int vertices)
{
	DeviceManager::m_pDeviceContext->Draw(vertices, 0);
}

void Renderer::DrawIndexed(unsigned int indices)
{
	DeviceManager::m_pDeviceContext->DrawIndexed(indices, 0, 0);
}

void Renderer::DrawIndexedInstanced(unsigned int indices, unsigned int instances)
{
	DeviceManager::m_pDeviceContext->DrawIndexedInstanced(indices, instances, 0, 0, 0);
}

RenderTarget* Renderer::GetCurrentRenderTarget() const
{
	return m_pCurrentRenderTarget;
}

void Renderer::SetCurrentRenderTarget(RenderTarget* pRenderTarget)
{
	m_pCurrentRenderTarget = (pRenderTarget != nullptr) ? pRenderTarget : m_pDefaultRenderTarget;

	ID3D11RenderTargetView* pRenderTargetView{ m_pCurrentRenderTarget->GetRenderTargetView() };
	DeviceManager::m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, m_pCurrentRenderTarget->GetDepthStencilView());
}

void Renderer::InitializeFrame()
{
	if (m_RenderStateChanged)
	{
		DeviceManager::m_pDeviceContext->RSSetState(m_pRasterizerState);
		m_RenderStateChanged = false;
	}

	const XMFLOAT3 directXClearColor{ 0.f, 0.f, 0.3f };
	DeviceManager::m_pDeviceContext->ClearRenderTargetView(m_pCurrentRenderTarget->GetRenderTargetView(), &directXClearColor.x); // clears the render target with clearColor
	DeviceManager::m_pDeviceContext->ClearDepthStencilView(m_pCurrentRenderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0); // clears the depth buffer with value 1 and the stencil buffer with value 0
}

void Renderer::PresentFrame()
{
	m_pSwapChain->Present(0, NULL); // swap the buffers, no synchronization and no options
	ID3D11RenderTargetView* pRenderTargetView{ m_pCurrentRenderTarget->GetRenderTargetView() };
	DeviceManager::m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, m_pCurrentRenderTarget->GetDepthStencilView()); // binds the backbuffer and depth stencil view back to the output-merger state (required because using DXGI_SWAP_EFFECT_FLIP_DISCARD) - https://devblogs.microsoft.com/directx/dxgi-flip-model/
}