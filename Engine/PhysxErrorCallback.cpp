#include "pch.h"
#include "PhysxErrorCallback.h"

void PhysxErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	Logging::Log(LogType::ERROR, "[Physx] - Code: " + std::to_string(code) + "\n\tMessage: " + message + "\n\tFile: " + file + "\n\tLine: " + std::to_string(line));
}