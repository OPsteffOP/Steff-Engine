#include "pch.h"
#include "TextureManager.h"
#include "DeviceManager.h"
#include <FreeImage.h>
#include <sstream>

using namespace SteffEngine::Core;

TextureManager* TextureManager::m_pInstance{ nullptr };

TextureManager* TextureManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new TextureManager{};
	}

	return m_pInstance;
}

TextureManager::TextureManager()
{
	FreeImage_Initialise(true);
}

void TextureManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

TextureManager::~TextureManager()
{
	FreeImage_DeInitialise();
}

Texture& TextureManager::GetTexture(const std::wstring& texturePath)
{
	return CreateIfNotExists(texturePath)->second;
}

std::unordered_map<std::wstring, Texture>::iterator TextureManager::CreateIfNotExists(const std::wstring& texturePath)
{
	// Will create the texture if it doesn't exists, otherwise it'll just return the texture that already exists
	std::unordered_map<std::wstring, Texture>::iterator foundIterator{ m_Textures.find(texturePath) };
	if (foundIterator == m_Textures.end())
	{
		foundIterator = m_Textures.emplace(std::piecewise_construct,
			std::forward_as_tuple(texturePath),
			std::forward_as_tuple(texturePath)).first;
	}

	return foundIterator;
}