#include "pch.h"
#include "ShaderManager.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

ShaderManager* ShaderManager::m_pInstance{ nullptr };

ShaderManager* ShaderManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new ShaderManager{};
	}

	return m_pInstance;
}

void ShaderManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

ShaderManager::~ShaderManager()
{
	std::for_each(m_pVertexShaders.begin(), m_pVertexShaders.end(),
		[](std::pair<const std::wstring, std::pair<ID3DBlob*, ID3D11VertexShader*>>& pPair) {
			SAFE_DELETE_DIRECTX(pPair.second.first); SAFE_DELETE_DIRECTX(pPair.second.second); });

	std::for_each(m_pGeometryShaders.begin(), m_pGeometryShaders.end(),
		[](std::pair<const std::wstring, std::pair<ID3DBlob*, ID3D11GeometryShader*>>& pPair) {
			SAFE_DELETE_DIRECTX(pPair.second.first); SAFE_DELETE_DIRECTX(pPair.second.second); });

	std::for_each(m_pPixelShaders.begin(), m_pPixelShaders.end(),
		[](std::pair<const std::wstring, std::pair<ID3DBlob*, ID3D11PixelShader*>>& pPair) {
			SAFE_DELETE_DIRECTX(pPair.second.first); SAFE_DELETE_DIRECTX(pPair.second.second); });
}

ID3DBlob* ShaderManager::LoadBlob(const std::wstring& compiledShaderPath) const
{
	ID3DBlob* pShaderBlob;
	HRESULT result{ D3DReadFileToBlob(compiledShaderPath.c_str(), &pShaderBlob) };
	if (FAILED(result))
	{
		Logging::Log(LogType::ERROR, "Failed to load shader blob");
	}

	return pShaderBlob;
}

const std::pair<ID3DBlob*, ID3D11VertexShader*>& ShaderManager::GetVertexShader(const std::wstring& compiledShaderPath)
{
	// Will create the shader if it doesn't exists, otherwise it'll just return the shader that already exists
	std::unordered_map<std::wstring, std::pair<ID3DBlob*, ID3D11VertexShader*>>::iterator foundShaderIterator{ 
		m_pVertexShaders.find(compiledShaderPath) };
	if (foundShaderIterator == m_pVertexShaders.cend())
	{
		ID3DBlob* pShaderBlob{ LoadBlob(compiledShaderPath) };

		ID3D11VertexShader* pVertexShader;
		HRESULT result{ DeviceManager::m_pDevice->CreateVertexShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &pVertexShader) };
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create vertex shader");
		}

		foundShaderIterator = m_pVertexShaders.emplace(compiledShaderPath, std::make_pair(pShaderBlob, pVertexShader)).first;
	}

	return foundShaderIterator->second;
}

const std::pair<ID3DBlob*, ID3D11GeometryShader*>& ShaderManager::GetGeometryShader(const std::wstring& compiledShaderPath)
{
	// Will create the shader if it doesn't exists, otherwise it'll just return the shader that already exists
	std::unordered_map<std::wstring, std::pair<ID3DBlob*, ID3D11GeometryShader*>>::iterator foundShaderIterator{
		m_pGeometryShaders.find(compiledShaderPath) };
	if (foundShaderIterator == m_pGeometryShaders.cend())
	{
		ID3DBlob* pShaderBlob{ LoadBlob(compiledShaderPath) };

		ID3D11GeometryShader* pGeometryShader;
		HRESULT result{ DeviceManager::m_pDevice->CreateGeometryShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &pGeometryShader) };
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create geometry shader");
		}

		foundShaderIterator = m_pGeometryShaders.emplace(compiledShaderPath, std::make_pair(pShaderBlob, pGeometryShader)).first;
	}

	return foundShaderIterator->second;
}

const std::pair<ID3DBlob*, ID3D11PixelShader*>& ShaderManager::GetPixelShader(const std::wstring& compiledShaderPath)
{
	// Will create the shader if it doesn't exists, otherwise it'll just return the shader that already exists
	std::unordered_map<std::wstring, std::pair<ID3DBlob*, ID3D11PixelShader*>>::iterator foundShaderIterator{
		m_pPixelShaders.find(compiledShaderPath) };
	if (foundShaderIterator == m_pPixelShaders.cend())
	{
		ID3DBlob* pShaderBlob{ LoadBlob(compiledShaderPath) };

		ID3D11PixelShader* pPixelShader;
		HRESULT result{ DeviceManager::m_pDevice->CreatePixelShader(pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(), nullptr, &pPixelShader) };
		if (FAILED(result))
		{
			Logging::Log(LogType::ERROR, "Failed to create pixel shader");
		}

		foundShaderIterator = m_pPixelShaders.emplace(compiledShaderPath, std::make_pair(pShaderBlob, pPixelShader)).first;
	}

	return foundShaderIterator->second;
}