#pragma once
#include "ExposedVariableData.h"
#include "GameObjectData.h"

namespace SteffEngine
{
	namespace Core
	{
		class GameObject;

		namespace Components
		{
			class Component;
		}
	}

	namespace Editor
	{
		class EditorSceneStorage
		{
		public:
			// Components
			static SteffEngine::Core::Components::Component* GetComponent(unsigned int id);
			static std::pair<size_t, const ExposedVariableData*> GetComponentData(unsigned int id);

			static void AddComponent(SteffEngine::Core::GameObject* pGameObject, SteffEngine::Core::Components::Component* pComponent);
			static void RemoveComponent(SteffEngine::Core::GameObject* pGameObject, SteffEngine::Core::Components::Component* pComponent);
			static void ClearComponents();

			// GameObjects
			static const std::vector<GameObjectData>& GetGameObjectsData();
			static SteffEngine::Core::GameObject* GetGameObject(unsigned int id);
			static const std::vector<unsigned int>& GetGameObjectComponents(unsigned int id);

			static void AddGameObject(SteffEngine::Core::GameObject* pGameObject);
			static void RemoveGameObject(SteffEngine::Core::GameObject* pGameObject);
			static void ClearGameObjects();

		private:
			static std::unordered_map<unsigned int, SteffEngine::Core::Components::Component*> m_pComponents;
			static std::unordered_map<SteffEngine::Core::Components::Component*, std::vector<ExposedVariableData>> m_pComponentsData;

			static std::unordered_map<unsigned int, SteffEngine::Core::GameObject*> m_pGameObjects;
			static std::vector<GameObjectData> m_GameObjectsData;
			static std::unordered_map<unsigned int, std::vector<unsigned int>> m_GameObjectComponentsLink;
		};
	}
}