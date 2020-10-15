#pragma once
#include <string_view>
#include <chrono>
#include <fstream>

namespace SteffEngine
{
	namespace Timers
	{
		class Benchmarking
		{
		public:
			PLUGIN_API explicit Benchmarking(const std::string_view& benchmarkName);
			PLUGIN_API void Stop();

			PLUGIN_API const std::string_view& GetBenchmarkName() const;

			template<typename T>
			PLUGIN_API auto GetDuration();

			PLUGIN_API friend std::ostream& operator<<(std::ostream& stream, Benchmarking& benchmark);

		private:
			const std::string_view m_BenchmarkName;

			const std::chrono::time_point<std::chrono::high_resolution_clock> m_StartPoint;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_EndPoint;
		};

		template<typename T>
		PLUGIN_API
		auto Benchmarking::GetDuration()
		{
			const std::chrono::time_point<std::chrono::high_resolution_clock> endPoint{ std::chrono::high_resolution_clock::now() };
			if (m_EndPoint.time_since_epoch().count() == 0LL)
			{
				m_EndPoint = endPoint;
			}

			const auto duration{ std::chrono::duration_cast<T>(m_EndPoint - m_StartPoint) };
			return duration.count();
		}

		PLUGIN_API
		std::ostream& operator<<(std::ostream& stream, Benchmarking& benchmarking)
		{
			const long long duration{ benchmarking.GetDuration<std::chrono::milliseconds>() };

			stream << benchmarking.m_BenchmarkName << ": " << duration << " milliseconds";
			return stream;
		}
	}
}