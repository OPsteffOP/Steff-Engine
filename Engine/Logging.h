#pragma once
#include <fstream>

#undef ERROR

namespace SteffEngine
{
	namespace Tools
	{
		enum class LogType 
		{
			INFORMATION,
			WARNING,
			ERROR
		};

		class Logging
		{
		public:
			PLUGIN_API static void Log(LogType logType, const std::string& message);
			PLUGIN_API static void Log(LogType logType, const std::wstring& message);

		private:
			static std::string GetPrefix(LogType logType);

			static std::ofstream m_LogFile;
		};
	}
}