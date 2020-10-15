#include "pch.h"
#include "Timer.h"

using namespace SteffEngine::Timers;

Timer::Timer()
	: m_StartPoint(std::chrono::high_resolution_clock::now())
	, m_IntermediateStartPoint(std::chrono::high_resolution_clock::now())
{

}

void Timer::UpdateIntermediateStartPoint()
{
	m_IntermediateStartPoint = std::chrono::high_resolution_clock::now();
}

PLUGIN_API
float Timer::GetTotalSec() const
{
	const std::chrono::high_resolution_clock::time_point endPoint{ std::chrono::high_resolution_clock::now() };
	return GetDuration(m_StartPoint, endPoint);
}

PLUGIN_API
float Timer::GetElapsedSec() const
{
	const std::chrono::high_resolution_clock::time_point endPoint{ std::chrono::high_resolution_clock::now() };
	return GetDuration(m_IntermediateStartPoint, endPoint);
}

float Timer::GetDuration(std::chrono::high_resolution_clock::time_point startPoint, std::chrono::high_resolution_clock::time_point endPoint) const
{
	const std::chrono::duration<float> totalSeconds{ endPoint - startPoint };
	return totalSeconds.count();
}