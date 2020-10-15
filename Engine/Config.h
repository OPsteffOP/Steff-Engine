#pragma once
#include <unordered_map>
#include <optional>

namespace SteffEngine
{
	namespace Core
	{
		class Config
		{
		public:
			explicit Config(const std::wstring& configFilePath);

			PLUGIN_API const std::unordered_map<std::wstring, std::wstring> GetData() const { return m_Data; };

			PLUGIN_API std::optional<std::reference_wrapper<const std::wstring>> GetValue(const std::wstring& key) const;
			PLUGIN_API void SetValue(const std::wstring& key, const std::wstring& value);

			PLUGIN_API void Clear();

			PLUGIN_API void Reload();
			PLUGIN_API bool Save() const;

		private:
			void LoadConfig();

			const std::wstring m_ConfigFilePath;
			std::unordered_map<std::wstring, std::wstring> m_Data;
		};
	}
}