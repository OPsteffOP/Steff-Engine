#pragma once
#include "CameraComponent.h"
#include <unordered_set>

namespace SteffEngine
{
	namespace Core
	{
		class CameraManager
		{
		public:
			PLUGIN_API static CameraManager* GetInstance();
			CameraManager(const CameraManager& cameraManager) = delete;
			CameraManager(CameraManager&& cameraManager) = delete;

			static void Destroy();

			CameraManager& operator=(const CameraManager& cameraManager) = delete;
			CameraManager& operator=(CameraManager&& cameraManager) = delete;

			PLUGIN_API inline Components::CameraComponent* GetActiveCamera() const { return m_pActiveCamera; };
			PLUGIN_API void SetActiveCamera(Components::CameraComponent* pCamera);

		private:
			explicit CameraManager();

			void SubscribeInput();

			static CameraManager* m_pInstance;

			Components::CameraComponent* m_pActiveCamera;
		};
	}
}