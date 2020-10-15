#include "pch.h"
#include "Config.h"
#include <fstream>
#include <string>

using namespace SteffEngine::Core;

Config::Config(const std::wstring& configFilePath)
	: m_ConfigFilePath(configFilePath)
{
	LoadConfig();
}

void Config::LoadConfig()
{
	std::wifstream input{ m_ConfigFilePath };
	if (!input)
		return;

	std::wstring line;
	while (std::getline(input, line))
	{
		if (line.starts_with(L"//") || line.empty())
			continue;

		const std::wstring delimiterString{ L": " };
		const size_t delimiter{ line.find(delimiterString) };
		m_Data.emplace(line.substr(0, delimiter), line.substr(delimiter + delimiterString.size()));
	}
}

PLUGIN_API
std::optional<std::reference_wrapper<const std::wstring>> Config::GetValue(const std::wstring& key) const
{
	const std::unordered_map<std::wstring, std::wstring>::const_iterator foundValueIterator{ m_Data.find(key) };
	if (foundValueIterator != m_Data.cend())
		return std::cref((*foundValueIterator).second);

	return std::nullopt;
}

PLUGIN_API
void Config::SetValue(const std::wstring& key, const std::wstring& value)
{
	m_Data[key] = value;
}

PLUGIN_API
void Config::Clear()
{
	m_Data.clear();
}

PLUGIN_API
void Config::Reload()
{
	m_Data.clear();
	LoadConfig();
}

PLUGIN_API
bool Config::Save() const
{
	std::wofstream output{ m_ConfigFilePath };
	if (!output)
		return false;

	for (const std::pair<std::wstring, std::wstring>& pair : m_Data)
	{
		output << pair.first << ": " << pair.second << '\n';
	}

	return true;
}