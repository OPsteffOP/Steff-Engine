#pragma once
#include "Renderer.h"

namespace SteffEngine
{
	namespace Debugging
	{
		PLUGIN_API void DrawLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint);
		PLUGIN_API void DrawPermanentLine(const XMFLOAT3& beginPoint, const XMFLOAT3& endPoint);

		void Update();
		void Render(SteffEngine::Core::Renderer& renderer);

		void CleanUp();
	}
}