#pragma once
#include "Timer.h"

namespace SteffEngine
{
	namespace Timers
	{
		class FrameTimer : public Timer
		{
		public:
			PLUGIN_API static FrameTimer* GetInstance();
			static void Destroy();

			PLUGIN_API unsigned int GetFPS() const;
			PLUGIN_API float GetElapsedSec() const override;

			void SetStartFrame();
			void SetEndFrame();

		private:
			explicit FrameTimer();

			static FrameTimer* m_pInstance;

			float m_ElapsedSec;
			float m_TotalSec;
			unsigned int m_Frames;
			unsigned int m_FPS;
		};
	}
}