#pragma once
#include "Texture.h"
#include <unordered_map>

namespace SteffEngine
{
	namespace Core
	{
		class TextureManager
		{
		public:
			static TextureManager* GetInstance();
			static void Destroy();

			Texture& GetTexture(const std::wstring& texturePath);

		private:
			TextureManager();
			~TextureManager();

			std::unordered_map<std::wstring, Texture>::iterator CreateIfNotExists(const std::wstring& texturePath);

			static TextureManager* m_pInstance;

			std::unordered_map<std::wstring, Texture> m_Textures;
		};
	}
}