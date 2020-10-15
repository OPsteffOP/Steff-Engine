#include "pch.h"
#include "ConfigManager.h"

using namespace SteffEngine::Core;

ConfigManager* ConfigManager::m_pInstance{ nullptr };

ConfigManager* ConfigManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new ConfigManager{};
	}

	return m_pInstance;
}

void ConfigManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

Config& ConfigManager::GetConfig(const std::wstring& configName)
{
	// Will create the config if it doesn't exists, otherwise it'll just return the config that already exists
	std::unordered_map<std::wstring, Config>::iterator configIterator{ m_Configs.find(configName) };
	if (configIterator == m_Configs.end())
	{
		configIterator = m_Configs.emplace(configName, Config{ CONFIG_FOLDER + configName }).first;
	}

	return configIterator->second;
}