#include "pch.h"
#include "Benchmarking.h"

using namespace SteffEngine::Timers;

PLUGIN_API
Benchmarking::Benchmarking(const std::string_view& benchmarkName)
	: m_BenchmarkName(benchmarkName)
	, m_StartPoint(std::chrono::high_resolution_clock::now())
{
	
}

PLUGIN_API
void Benchmarking::Stop()
{
	m_EndPoint = std::chrono::high_resolution_clock::now();
}

PLUGIN_API
const std::string_view& Benchmarking::GetBenchmarkName() const
{
	return m_BenchmarkName;
}