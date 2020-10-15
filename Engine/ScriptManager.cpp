#include "pch.h"
#include "ScriptManager.h"
#include <filesystem>

using namespace SteffEngine::Core;
using namespace SteffEngine::Scripting;

const std::string ScriptManager::m_ScriptFolderPath{ "C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Resources\\Scripts\\" };
ScriptManager* ScriptManager::m_pInstance{ nullptr };

ScriptManager* ScriptManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new ScriptManager{};
	}

	return m_pInstance;
}

void ScriptManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

ScriptManager::~ScriptManager()
{
	UnloadScripts();
}

void ScriptManager::LoadScripts()
{
	if (!m_pScripts.empty())
	{
		ReloadScripts();
		return;
	}

	Logging::Log(LogType::INFORMATION, "Loading scripts...");

	Config& scriptsConfig{ ConfigManager::GetInstance()->GetConfig(L"Scripts.conf") };
	for (const std::filesystem::directory_entry& directoryEntry : std::filesystem::directory_iterator(m_ScriptFolderPath))
	{
		if (!directoryEntry.is_directory())
		{
			const std::filesystem::path path{ directoryEntry.path() };
			
			const std::optional<std::reference_wrapper<const std::wstring>> isScriptEnabled{ scriptsConfig.GetValue(path.filename().wstring()) };
			if (isScriptEnabled.has_value() && isScriptEnabled->get() == L"true")
			{
				if (path.has_extension() && path.extension().wstring() == L".dll")
				{
					HMODULE scriptDll{ LoadLibrary(path.wstring().c_str()) };
					if (!scriptDll)
					{
						Logging::Log(LogType::ERROR, L"Couldn't load DLL: " + path.wstring());
						return;
					}

					Logging::Log(LogType::INFORMATION, L"Loading DLL: " + path.wstring());
					m_pScripts.push_back(new Script{ scriptDll });
					Logging::Log(LogType::INFORMATION, L"DLL loaded: " + path.wstring());
				}
			}
		}
	}

	Logging::Log(LogType::INFORMATION, "Scripts loaded");
}

void ScriptManager::UnloadScripts()
{
	Logging::Log(LogType::INFORMATION, "Unloading scripts...");

	std::for_each(m_pScripts.begin(), m_pScripts.end(), [](Script* pScript) { SAFE_DELETE(pScript); });

	Logging::Log(LogType::INFORMATION, "Scripts unloaded");
}

void ScriptManager::ReloadScripts()
{
	Logging::Log(LogType::INFORMATION, "Reloading scripts...");

	UnloadScripts();
	LoadScripts();

	Logging::Log(LogType::INFORMATION, "Scripts reloaded");
}