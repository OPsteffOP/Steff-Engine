#include "pch.h"
#include "FrameTimer.h"

using namespace SteffEngine::Timers;

FrameTimer* FrameTimer::m_pInstance{ nullptr };

PLUGIN_API
FrameTimer* FrameTimer::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new FrameTimer{};
    }

    return m_pInstance;
}

FrameTimer::FrameTimer()
    : Timer()
    , m_ElapsedSec(0.f)
    , m_TotalSec(0.f)
    , m_Frames(0)
    , m_FPS(0)
{
    Timer::UpdateIntermediateStartPoint();
}

void FrameTimer::Destroy()
{
    SAFE_DELETE(m_pInstance);
}

PLUGIN_API
unsigned int FrameTimer::GetFPS() const
{
    return m_FPS;
}

PLUGIN_API
float FrameTimer::GetElapsedSec() const
{
    return m_ElapsedSec;
}

void FrameTimer::SetStartFrame()
{
    Timer::UpdateIntermediateStartPoint();
}

void FrameTimer::SetEndFrame()
{
    m_ElapsedSec = Timer::GetElapsedSec();

    ++m_Frames;
    m_TotalSec += m_ElapsedSec;

    if (m_TotalSec >= 1.f)
    {
        m_FPS = m_Frames;

        m_Frames = 0;
        m_TotalSec = 0.f;
    }
}