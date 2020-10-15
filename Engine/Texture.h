#pragma once
namespace SteffEngine
{
	namespace Core
	{
		class Texture
		{
		public:
			explicit Texture(const std::wstring& texturePath);
			Texture(const Texture& texture) = delete;
			Texture(Texture&& texture) = delete;

			~Texture();

			Texture& operator=(const Texture& texture) = delete;
			Texture& operator=(Texture&& texture) = delete;

			ID3D11Texture2D* GetTexture() const;
			ID3D11ShaderResourceView* GetShaderResourceView() const;

			inline const XMFLOAT2& GetSize() const { return m_Size; };

		private:
			bool CreateTexture(const std::wstring& texturePath);

			ID3D11Texture2D* m_pTexture;
			ID3D11ShaderResourceView* m_pShaderResourceView;

			XMFLOAT2 m_Size;
		};
	}
}