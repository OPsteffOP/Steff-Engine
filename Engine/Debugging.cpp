#include "pch.h"
#include "Debugging.h"
#include "LineDebugging.h"
#include "UIDebugging.h"

PLUGIN_API
void SteffEngine::Debugging::DrawLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint)
{
	LineDebugging::GetInstance()->AddLine(beginPoint, endPoint);
}

PLUGIN_API
void SteffEngine::Debugging::DrawPermanentLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint)
{
	LineDebugging::GetInstance()->AddPermanentLine(beginPoint, endPoint);
}

void SteffEngine::Debugging::Update()
{
	LineDebugging::GetInstance()->Update();
}

void SteffEngine::Debugging::Render(SteffEngine::Core::Renderer& renderer)
{
	LineDebugging::GetInstance()->Render(renderer);
	UIDebugging::GetInstance()->Render(renderer);
}

void SteffEngine::Debugging::CleanUp()
{
	LineDebugging::Destroy();
	UIDebugging::Destroy();
}