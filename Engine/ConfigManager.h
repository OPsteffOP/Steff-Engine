#pragma once
#include "Config.h"
#include <unordered_map>

namespace SteffEngine
{
	namespace Core
	{
		class ConfigManager
		{
		public:
			static ConfigManager* GetInstance();
			static void Destroy();

			Config& GetConfig(const std::wstring& configName);

		private:
			ConfigManager() = default;

			static ConfigManager* m_pInstance;

			std::unordered_map<std::wstring, Config> m_Configs;
		};
	}
}