#include "pch.h"
#include "UIDebugging.h"
#include "FrameTimer.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Debugging;
using namespace SteffEngine::Timers;

UIDebugging* UIDebugging::m_pInstance{ nullptr };

UIDebugging* UIDebugging::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new UIDebugging{};
	}

	return m_pInstance;
}

void UIDebugging::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

void UIDebugging::Render(Renderer& renderer)
{
	ImGui::Begin("PERFORMANCE");
	ImGui::Text(("FPS: " + std::to_string(FrameTimer::GetInstance()->GetFPS())).c_str());
	ImGui::Spacing();
	ImGui::Text("");
	ImGui::End();
}