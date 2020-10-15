#pragma once
#include <unordered_map>
#include <any>

template<typename T, typename... U>
using DLL_FUNCTION = T(*)(U...);

namespace SteffEngine
{
	namespace Scripting
	{
		class Script
		{
		public:
			explicit Script(HMODULE library);
			Script(const Script& script) = delete;
			Script(Script&& script) = delete;

			~Script();

			Script& operator=(const Script& script) = delete;
			Script& operator=(Script&& script) = delete;

			template<typename T, typename... U>
			std::optional<T> CallFunction(const std::string& functionName, U&&... parameters) const;

			template<typename... U>
			void CallFunction(const std::string& functionName, U&&... parameters) const;

		private:
			void LoadFunctions();

			template<typename T, typename... U>
			void LoadFunction(const std::string& functionName);

			HMODULE m_Library;
			std::unordered_map<std::string, std::any> m_Functions;
		};

		template<typename T, typename... U>
		void Script::LoadFunction(const std::string& functionName)
		{
			DLL_FUNCTION<T, U...> function{ (DLL_FUNCTION<T, U...>)GetProcAddress(m_Library, functionName.c_str()) };
			if (function)
			{
				Logging::Log(LogType::INFORMATION, "Function: '" + functionName + "' loaded");
				m_Functions.emplace(functionName, function);
			}
		}

		template<typename T, typename... U>
		std::optional<T> Script::CallFunction(const std::string& functionName, U&&... parameters) const
		{
			const std::unordered_map<std::string, std::any>::const_iterator foundFunctionIterator{ m_Functions.find(functionName) };
			if (foundFunctionIterator == m_Functions.cend())
				return std::nullopt;

			return std::make_optional(std::any_cast<DLL_FUNCTION<T, U...>>(foundFunctionIterator->second)(std::forward<U>(parameters)...));
		}

		template<typename... U>
		void Script::CallFunction(const std::string& functionName, U&&... parameters) const
		{
			const std::unordered_map<std::string, std::any>::const_iterator foundFunctionIterator{ m_Functions.find(functionName) };
			if (foundFunctionIterator == m_Functions.cend())
				return;

			std::any_cast<DLL_FUNCTION<void, U...>>(foundFunctionIterator->second)(std::forward<U>(parameters)...);
		}
	}
}