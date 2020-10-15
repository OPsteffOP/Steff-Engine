#include "pch.h"
#include "RenderTarget.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

RenderTarget::RenderTarget(ID3D11Texture2D* pBuffer)
	: m_pRenderTargetBuffer(pBuffer)
	, m_pRenderTargetView(nullptr)
{
	
}

RenderTarget::~RenderTarget()
{
	SAFE_DELETE_DIRECTX(m_pRenderTargetView);
	SAFE_DELETE_DIRECTX(m_pRenderTargetBuffer);
	SAFE_DELETE_DIRECTX(m_pColorShaderResourceView);
	SAFE_DELETE_DIRECTX(m_pDepthStencilView);
	SAFE_DELETE_DIRECTX(m_pDepthStencilBuffer);
	SAFE_DELETE_DIRECTX(m_pDepthShaderResourceView);
}

void RenderTarget::Create(RenderTargetDesc& desc)
{
	// =-=-=-=-= CREATING COLOR BUFFER =-=-=-=-= //
	if (m_pRenderTargetBuffer == nullptr)
	{
		D3D11_TEXTURE2D_DESC textureDescriptor{};
		textureDescriptor.Width = WINDOW_WIDTH;
		textureDescriptor.Height = WINDOW_HEIGHT;
		textureDescriptor.MipLevels = 1; // no mipmapping
		textureDescriptor.ArraySize = 1;
		textureDescriptor.Format = desc.color.format;
		textureDescriptor.SampleDesc.Count = 1; // no anti-aliasing
		textureDescriptor.SampleDesc.Quality = 0;
		textureDescriptor.Usage = D3D11_USAGE_DEFAULT; // read and write access by the GPU
		textureDescriptor.BindFlags = D3D11_BIND_RENDER_TARGET | ((desc.color.bindableShaderResource) ? D3D11_BIND_SHADER_RESOURCE : 0); // binds texture to a shader stage (and possibly to a shader)
		textureDescriptor.CPUAccessFlags = NULL;
		textureDescriptor.MiscFlags = NULL;

		HRESULT result = DeviceManager::m_pDevice->CreateTexture2D(&textureDescriptor, nullptr, &m_pRenderTargetBuffer);
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create texture buffer for render target");
		}
	}

	// pDesc is NULL to initialize a default descriptor
	HRESULT result = DeviceManager::m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer, NULL, &m_pRenderTargetView);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create render target view");
		return;
	}

	if (desc.color.bindableShaderResource)
	{
		result = DeviceManager::m_pDevice->CreateShaderResourceView(m_pRenderTargetBuffer, NULL, &m_pColorShaderResourceView);
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create color shader resource view for render target");
			return;
		}
	}

	// =-=-=-=-= CREATING DEPTH-STENCIL BUFFER =-=-=-=-= //
	D3D11_TEXTURE2D_DESC depthStencilDescriptor{};
	depthStencilDescriptor.Width = WINDOW_WIDTH; // texture width
	depthStencilDescriptor.Height = WINDOW_HEIGHT; // texture height
	depthStencilDescriptor.MipLevels = 1; // multisampled texture - https://www.khronos.org/opengl/wiki/Multisample_Texture
	depthStencilDescriptor.ArraySize = 1; // number of textures in the texture array
	depthStencilDescriptor.Format = GetDepthResourceFormat(desc.depth.format);

	// default sampler mode with no anti-aliasing - https://en.wikipedia.org/wiki/Multisample_anti-aliasing
	depthStencilDescriptor.SampleDesc.Count = 1;
	depthStencilDescriptor.SampleDesc.Quality = 0;

	depthStencilDescriptor.Usage = D3D11_USAGE_DEFAULT; // allows read and write access for the GPU
	depthStencilDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | ((desc.depth.bindableShaderResource) ? D3D11_BIND_SHADER_RESOURCE : 0); // "Bind a texture as a depth-stencil target for the output-merger stage." (see stages for output-merger stage information)
	depthStencilDescriptor.CPUAccessFlags = NULL;
	depthStencilDescriptor.MiscFlags = NULL;

	// pInitialData has to be NULL because we're using a multisampled resource
	result = DeviceManager::m_pDevice->CreateTexture2D(&depthStencilDescriptor, NULL, &m_pDepthStencilBuffer);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create depth stencil texture");
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor{};
	depthStencilViewDescriptor.Format = desc.depth.format;
	depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // resource will be accessed as a 2D texture
	depthStencilViewDescriptor.Texture2D.MipSlice = 0; // index of the first mipmap level to use

	result = DeviceManager::m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDescriptor, &m_pDepthStencilView);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create depth stencil view");
		return;
	}

	if (desc.depth.bindableShaderResource)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc;
		ZeroMemory(&depthSrvDesc, sizeof(depthSrvDesc));

		depthSrvDesc.Format = GetDepthSRVFormat(desc.depth.format);
		depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthSrvDesc.Texture2D.MipLevels = 1;
		depthSrvDesc.Texture2D.MostDetailedMip = 0;

		result = DeviceManager::m_pDevice->CreateShaderResourceView(m_pDepthStencilBuffer, &depthSrvDesc, &m_pDepthShaderResourceView);
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create depth shader resource view for render target");
			return;
		}
	}
}

ID3D11Texture2D* RenderTarget::GetColorBuffer() const
{
	return m_pRenderTargetBuffer;
}

ID3D11RenderTargetView* RenderTarget::GetRenderTargetView() const
{
	return m_pRenderTargetView;
}

ID3D11ShaderResourceView* RenderTarget::GetColorShaderResourceView() const
{
	return m_pColorShaderResourceView;
}

ID3D11Texture2D* RenderTarget::GetDepthStencilBuffer() const
{
	return m_pDepthStencilBuffer;
}

ID3D11DepthStencilView* RenderTarget::GetDepthStencilView() const
{
	return m_pDepthStencilView;
}

ID3D11ShaderResourceView* RenderTarget::GetDepthShaderResourceView() const
{
	return m_pDepthShaderResourceView;
}

DXGI_FORMAT RenderTarget::GetDepthResourceFormat(DXGI_FORMAT initFormat)
{
	DXGI_FORMAT resourceFormat = {};
	switch (initFormat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		resourceFormat = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
		break;
	default:
		Logging::Log(LogType::ERROR, "Depth resource format not supported!");
		break;
	}

	return resourceFormat;
}

DXGI_FORMAT RenderTarget::GetDepthSRVFormat(DXGI_FORMAT initFormat)
{
	DXGI_FORMAT srvFormat = {};
	switch (initFormat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		srvFormat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		srvFormat = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		srvFormat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		srvFormat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	default:
		Logging::Log(LogType::ERROR, "Depth SRV format not supported!");
		break;
	}

	return srvFormat;
}