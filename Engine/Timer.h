#pragma once
#include <chrono>

namespace SteffEngine
{
	namespace Timers
	{
		class Timer
		{
		public:
			explicit Timer();

			void UpdateIntermediateStartPoint();

			PLUGIN_API float GetTotalSec() const;
			PLUGIN_API virtual float GetElapsedSec() const;

		protected:

		private:
			float GetDuration(std::chrono::high_resolution_clock::time_point startPoint, std::chrono::high_resolution_clock::time_point endPoint) const;

			const std::chrono::high_resolution_clock::time_point m_StartPoint;
			std::chrono::high_resolution_clock::time_point m_IntermediateStartPoint;
		};
	}
}