#include "pch.h"
#include "Logging.h"

using namespace SteffEngine::Tools;

std::ofstream Logging::m_LogFile{ "C:\\Users\\Administrator\\Perforce\\STEFF-DESKTOP\\Personal\\Engine\\Dev\\Log.txt" };

PLUGIN_API
void Logging::Log(LogType logType, const std::string& message)
{
	const std::string prefix{ GetPrefix(logType) };
	m_LogFile << prefix << message << std::endl;
}

PLUGIN_API
void Logging::Log(LogType logType, const std::wstring& message)
{
#pragma warning(disable: 4244)
	const std::string prefix{ GetPrefix(logType) };
	m_LogFile << prefix << std::string(message.begin(), message.end()) << std::endl;
#pragma warning(default: 4244)
}

std::string Logging::GetPrefix(LogType logType)
{
	switch (logType)
	{
	case LogType::INFORMATION:
		return "[INFO] ";
	case LogType::WARNING:
		return "[WARNING] ";
	case LogType::ERROR:
		return "[ERROR] ";
	}

	return "[UNKNOWN] ";
}