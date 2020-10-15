#include "pch.h"
#include "EditorShaderStorage.h"

#include <filesystem>

using namespace SteffEngine::Core;
using namespace SteffEngine::Editor;

unsigned int EditorShaderStorage::m_NextShaderID{ 1 };
std::unordered_map<unsigned int, ShaderData> EditorShaderStorage::m_ShaderData{};

void EditorShaderStorage::Initialize()
{
	// Load default shader
	LoadShader("DefaultShader", std::make_optional<unsigned int>(0));
}

unsigned int EditorShaderStorage::AddShaderData(ShaderData&& shaderData)
{
	const unsigned int shaderID{ m_NextShaderID++ };
	m_ShaderData.emplace(shaderID, std::move(shaderData));

	const std::wstring shaderInformationFileName{ StringToWString(shaderData.GetShaderName()) + L".sSE" };
	const std::wstring shaderFileName{ StringToWString(std::filesystem::path(shaderData.GetRawCompiledShaderPath()).filename().string()) };

	// Create shader config
	ConfigManager::GetInstance()->GetConfig(shaderInformationFileName).Clear();
	ConfigManager::GetInstance()->GetConfig(shaderInformationFileName).SetValue(L"Shader", shaderFileName);
	ConfigManager::GetInstance()->GetConfig(shaderInformationFileName).Save();

	return shaderID;
}

unsigned int EditorShaderStorage::AddShaderData(const std::string& shaderName, const std::string& compiledShaderPath)
{
	return AddShaderData(ShaderData{ shaderName, compiledShaderPath });
}

unsigned int EditorShaderStorage::AddShaderData(const std::string& shaderName, const std::string& compiledShaderPath, const std::string& value, ShaderType shaderType)
{
	const unsigned int id = AddShaderData(ShaderData{ shaderName, compiledShaderPath });
	m_ShaderData.find(id)->second.AddProperty(value, shaderType);
	return id;
}

unsigned int EditorShaderStorage::AddShaderData(const std::string& shaderName, const std::string& compiledShaderPath, const std::wstring& value, ShaderType shaderType)
{
	const unsigned int id = AddShaderData(ShaderData{ shaderName, compiledShaderPath });
	m_ShaderData.find(id)->second.AddProperty(value, shaderType);
	return id;
}

void EditorShaderStorage::AddProperty(unsigned int id, const std::string& value, ShaderType shaderType)
{
	AddProperty(id, StringToWString(value), shaderType);
}

void EditorShaderStorage::AddProperty(unsigned int id, const std::wstring& value, ShaderType shaderType)
{
	assert(m_ShaderData.find(id) != m_ShaderData.end()); // shader with id not found

	ShaderData& shaderData = m_ShaderData.find(id)->second;
	shaderData.AddProperty(value, shaderType);

	// Add data to shader config
	const std::wstring shaderInformationFileName{ StringToWString(shaderData.GetShaderName()) + L".sSE" };
	ConfigManager::GetInstance()->GetConfig(shaderInformationFileName).SetValue(value, std::to_wstring(int(shaderType)));
	ConfigManager::GetInstance()->GetConfig(shaderInformationFileName).Save();
}

const ShaderData& EditorShaderStorage::GetShaderData(const std::string& shaderName)
{
	std::unordered_map<unsigned int, ShaderData>::iterator foundShaderDataIterator{ std::find_if(m_ShaderData.begin(), m_ShaderData.end(),
		[&shaderName](const std::pair<unsigned int, ShaderData>& pair) { return pair.second.GetShaderName() == shaderName; }) };
	if (foundShaderDataIterator != m_ShaderData.end())
	{
		// load the shader with this name
		const unsigned int shaderID = LoadShader(shaderName);
		foundShaderDataIterator = m_ShaderData.find(shaderID);
	}

	return foundShaderDataIterator->second;
}

const ShaderData& EditorShaderStorage::GetShaderData(unsigned int id)
{
	const std::unordered_map<unsigned int, ShaderData>::iterator foundShaderDataIterator{ m_ShaderData.find(id) };
	assert(foundShaderDataIterator != m_ShaderData.end()); // shader with id not found

	return foundShaderDataIterator->second;
}

std::pair<const char*, int> EditorShaderStorage::CompileShader(const char* shaderPath, ShaderType type)
{
	std::pair<const char*, int> errorMessagePair{};
	errorMessagePair.second = -1;

	// Compiling the hlsl shader
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	flags |= D3DCOMPILE_DEBUG;

	LPCSTR profile{ "" };
	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
		profile = "vs_5_0\0";
		break;
		//case ShaderType::GEOMETRY_SHADER:
		//	profile = "gs_5_0\0";
		//	break;
	case ShaderType::PIXEL_SHADER:
		profile = "ps_5_0\0";
		break;
	default:
		assert(false); // unknown shader type
		break;
	}

	ID3DBlob* pShaderBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	HRESULT result = D3DCompileFromFile(StringToWString(shaderPath).c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main\0", profile, flags, NULL, &pShaderBlob, &pErrorBlob);
	if (result != S_OK)
	{
		errorMessagePair.second = 0;
		if (pErrorBlob != nullptr)
		{
			errorMessagePair.first = static_cast<const char*>(pErrorBlob->GetBufferPointer());
			errorMessagePair.second = int(pErrorBlob->GetBufferSize());
		}

		return errorMessagePair;
	}

	// Writing compiled shader to cso file
	std::string csoFilePath = shaderPath;
	csoFilePath.erase(csoFilePath.rfind('.'));
	csoFilePath += ".cso";

	result = D3DWriteBlobToFile(pShaderBlob, StringToWString(csoFilePath).c_str(), true);
	if (result != S_OK)
	{
		errorMessagePair.second = 0;
		if (pErrorBlob != nullptr)
		{
			errorMessagePair.first = static_cast<const char*>(pErrorBlob->GetBufferPointer());
			errorMessagePair.second = int(pErrorBlob->GetBufferSize());
		}

		return errorMessagePair;
	}

	return errorMessagePair;
}

unsigned int EditorShaderStorage::LoadShader(const std::string& shaderName)
{
	return LoadShader(shaderName, std::nullopt);
}

unsigned int EditorShaderStorage::LoadShader(const std::string& shaderName, std::optional<unsigned int> shaderID)
{
	if (shaderID != std::nullopt && m_ShaderData.find(*shaderID) != m_ShaderData.end())
		return *shaderID; // shader id already exists

	const std::unordered_map<unsigned int, ShaderData>::iterator foundShaderDataIterator{ std::find_if(m_ShaderData.begin(), m_ShaderData.end(),
	[&shaderName](const std::pair<unsigned int, ShaderData>& pair) { return pair.second.GetShaderName() == shaderName; }) };
	if (foundShaderDataIterator != m_ShaderData.end())
		return foundShaderDataIterator->first; // shader is already loaded - just return the id

	if (!std::filesystem::exists(CONFIG_FOLDER + StringToWString(shaderName + ".sSE")))
		return 0;

	const unsigned int createdShaderID{ (shaderID != std::nullopt) ? *shaderID : m_NextShaderID++ };

	const Config& shaderDataConfig{ ConfigManager::GetInstance()->GetConfig(StringToWString(shaderName) + L".sSE") };
	m_ShaderData.emplace(createdShaderID, ShaderData{ "", WStringToString(SHADERS_FOLDER + shaderDataConfig.GetValue(L"Shader")->get()) });
	for (const std::pair<std::wstring, std::wstring>& pair : shaderDataConfig.GetData())
	{
		if (pair.first != L"Shader" && pair.first != L"ID")
			m_ShaderData.find(createdShaderID)->second.AddProperty(pair.first, ShaderType(std::stoi(pair.second)));
	}

	return createdShaderID;
}