#pragma once
#include "Scene.h"
#include <unordered_map>

namespace SteffEngine
{
	namespace Core
	{
		class SceneManager
		{
		public:
			PLUGIN_API static SceneManager* GetInstance();
			static void Destroy();

			PLUGIN_API Scene* CreateScene(const std::string& sceneName);
			PLUGIN_API Scene* GetScene(const std::string& sceneName);

			PLUGIN_API void SetActiveScene(const std::string& sceneName);
			PLUGIN_API Scene* GetActiveScene();

			void FixedUpdate();
			void Update();
			void LateUpdate();
			void Render(Renderer& renderer);

		private:
			SceneManager();
			~SceneManager();

			static SceneManager* m_pInstance;

			std::unordered_map<std::string, Scene*> m_pScenes;
			Scene* m_pActiveScene;
		};
	}
}