#pragma once
#include "FrameTimer.h"
#include "Renderer.h"
#include "Scene.h"
#include <thread>

namespace SteffEngine
{
	namespace Core
	{
		class GameManager
		{
		public:
			PLUGIN_API static GameManager* GetInstance();
			static void Destroy();

			void Initialize(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight);
			void Tick();

		private:
			explicit GameManager();
			~GameManager();

			void Update();
			void Render();

			static GameManager* m_pInstance;

			Renderer* m_pRenderer;
		};
	}
}