#include "pch.h"
#include "Texture.h"
#include "DeviceManager.h"
#include <FreeImage.h>

using namespace SteffEngine::Core;

Texture::Texture(const std::wstring& texturePath)
{
	Logging::Log(LogType::INFORMATION, L"Loading texture: " + texturePath);

	if (CreateTexture(texturePath))
		Logging::Log(LogType::INFORMATION, L"Texture loaded: " + texturePath);
}

Texture::~Texture()
{
	SAFE_DELETE_DIRECTX(m_pTexture);
	SAFE_DELETE_DIRECTX(m_pShaderResourceView);
}

ID3D11Texture2D* Texture::GetTexture() const
{
	return m_pTexture;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
	return m_pShaderResourceView;
}

bool Texture::CreateTexture(const std::wstring& texturePath)
{
	HRESULT result;

	// Load texture (using FreeImage library)
	FIBITMAP* pFreeImageTexture{ FreeImage_LoadU(FreeImage_GetFileTypeU(texturePath.c_str()), texturePath.c_str()) };

	if (FreeImage_GetBPP(pFreeImageTexture) != 32)
	{
		FIBITMAP* pOldTexture{ pFreeImageTexture };
		pFreeImageTexture = FreeImage_ConvertTo32Bits(pOldTexture);
		FreeImage_Unload(pOldTexture);
	}

	// Create the descriptor for the texture
	D3D11_TEXTURE2D_DESC textureDescriptor{};
	textureDescriptor.Width = FreeImage_GetWidth(pFreeImageTexture);
	textureDescriptor.Height = FreeImage_GetHeight(pFreeImageTexture);
	textureDescriptor.MipLevels = 1; // no mipmapping
	textureDescriptor.ArraySize = 1;
	textureDescriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDescriptor.SampleDesc.Count = 1; // no anti-aliasing
	textureDescriptor.SampleDesc.Quality = 0;
	textureDescriptor.Usage = D3D11_USAGE_DEFAULT; // read and write access by the GPU
	textureDescriptor.BindFlags = D3D11_BIND_SHADER_RESOURCE; // binds texture to a shader stage
	textureDescriptor.CPUAccessFlags = NULL;
	textureDescriptor.MiscFlags = NULL;

	// Create the init data for the texture
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = FreeImage_GetBits(pFreeImageTexture);
	initData.SysMemPitch = FreeImage_GetPitch(pFreeImageTexture);

	// Create the texture for directx usage
	result = DeviceManager::m_pDevice->CreateTexture2D(&textureDescriptor, &initData, &m_pTexture);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create texture");
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescriptor{};
	shaderResourceViewDescriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	shaderResourceViewDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDescriptor.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDescriptor.Texture2D.MipLevels = 1;

	result = DeviceManager::m_pDevice->CreateShaderResourceView(m_pTexture, &shaderResourceViewDescriptor, &m_pShaderResourceView);
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to create diffuse shader resource view");
		return false;
	}

	m_Size.x = float(FreeImage_GetWidth(pFreeImageTexture));
	m_Size.y = float(FreeImage_GetHeight(pFreeImageTexture));

	// Unload FreeImage texture
	FreeImage_Unload(pFreeImageTexture);

	return true;
}