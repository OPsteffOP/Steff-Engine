#pragma once
#include "Renderer.h"

namespace SteffEngine
{
	namespace Debugging
	{
		class UIDebugging
		{
		public:
			static UIDebugging* GetInstance();
			static void Destroy();

			void Render(SteffEngine::Core::Renderer& renderer);

		private:
			static UIDebugging* m_pInstance;
		};
	}
}