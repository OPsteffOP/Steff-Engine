#pragma once
#include "Script.h"

namespace SteffEngine
{
	namespace Scripting
	{
		class ScriptManager
		{
		public:
			static ScriptManager* GetInstance();
			static void Destroy();

			void LoadScripts();
			void ReloadScripts();

			template<typename T, typename... U>
			std::vector<T> CallFunction(const std::string& functionName, U&&... parameters) const;

		private:
			ScriptManager() = default;
			~ScriptManager();

			void UnloadScripts();

			static const std::string m_ScriptFolderPath;
			static ScriptManager* m_pInstance;

			std::vector<Script*> m_pScripts;
		};

		template<typename T, typename... U>
		std::vector<T> ScriptManager::CallFunction(const std::string& functionName, U&&... parameters) const
		{
			std::vector<T> returnValues{};

			std::for_each(m_pScripts.cbegin(), m_pScripts.cend(), [&functionName, &returnValues, &parameters...](const Script* pScript) 
			{
				const std::optional<T> returnValue{ pScript->CallFunction<T, U...>(functionName, std::forward<U>(parameters)...) };
				if (returnValue != std::nullopt)
					returnValues.push_back(returnValue.value());
			});

			return returnValues;
		}
	}
}