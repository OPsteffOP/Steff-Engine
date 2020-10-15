#pragma once
#include "Shader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include <unordered_map>

namespace SteffEngine
{
	namespace Core
	{
		class ShaderManager
		{
		public:
			static ShaderManager* GetInstance();
			static void Destroy();

			const std::pair<ID3DBlob*, ID3D11VertexShader*>& GetVertexShader(const std::wstring& compiledShaderPath);
			const std::pair<ID3DBlob*, ID3D11GeometryShader*>& GetGeometryShader(const std::wstring& compiledShaderPath);
			const std::pair<ID3DBlob*, ID3D11PixelShader*>& GetPixelShader(const std::wstring& compiledShaderPath);

		private:
			ShaderManager() = default;
			~ShaderManager();

			ID3DBlob* LoadBlob(const std::wstring& compiledShaderPath) const;

			static ShaderManager* m_pInstance;

			std::unordered_map<std::wstring, std::pair<ID3DBlob*, ID3D11VertexShader*>> m_pVertexShaders;
			std::unordered_map<std::wstring, std::pair<ID3DBlob*, ID3D11GeometryShader*>> m_pGeometryShaders;
			std::unordered_map<std::wstring, std::pair<ID3DBlob*, ID3D11PixelShader*>> m_pPixelShaders;
		};
	}
}