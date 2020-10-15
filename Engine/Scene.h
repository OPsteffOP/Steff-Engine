#pragma once
#include "GameObject.h"
#include <PxScene.h>

namespace SteffEngine
{
	namespace Core
	{
		class PostProcessingEffect;
		class MeshData;

		class Scene
		{
		public:
			Scene();
			Scene(const Scene& scene) = delete;
			Scene(Scene&& scene) = delete;

			~Scene();

			Scene& operator=(const Scene& scene) = delete;
			Scene& operator=(Scene&& scene) = delete;

			PLUGIN_API GameObject* CreateGameObject(const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = { 1.f, 1.f, 1.f });
			PLUGIN_API GameObject* CreateGameObject(String name, const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = { 1.f, 1.f, 1.f });

			PLUGIN_API void AddGameObject(GameObject* pGameObject);
			PLUGIN_API void RemoveGameObject(GameObject* pGameObject, bool shouldDelete = true);

			PLUGIN_API void AddPostProcessingEffect(PostProcessingEffect* pPostProcessingEffect);
			PLUGIN_API void RemovePostProcessingEffect(PostProcessingEffect* pPostProcessingEffect, bool shouldDelete = true);

			void FixedUpdate();
			void Update();
			void LateUpdate();
			void Render(Renderer& renderer) const;

			PLUGIN_API physx::PxScene* GetPhysicsScene() const;
			PLUGIN_API physx::PxControllerManager* GetControllerManager() const;
			PLUGIN_API std::vector<GameObject*>& GetGameObjects();

		private:
			void InitializePhysx();
			void InitializePostProcessing();

			std::vector<GameObject*> m_pGameObjects;

			const MeshData* m_pPostProcessingMesh;
			std::vector<PostProcessingEffect*> m_pPostProcessingStack;

			physx::PxScene* m_pPhysicsScene;
			physx::PxControllerManager* m_pControllerManager;
		};
	}
}