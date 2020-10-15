#pragma once
#include "Component.h"
#include "TransformComponent.h"

namespace SteffEngine
{
	namespace Core
	{
		class Scene;

		namespace Components
		{
			class RigidBodyComponent;
			class CharacterControllerComponent;
		}

		class GameObject final
		{
		public:
			PLUGIN_API GameObject(const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = { 1.f, 1.f, 1.f });
			PLUGIN_API GameObject(String name, const XMFLOAT3& position = { 0.f, 0.f, 0.f }, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = { 1.f, 1.f, 1.f });
			GameObject(const GameObject& gameObject) = delete;
			GameObject(GameObject&& gameObject) = delete;

			~GameObject();

			GameObject& operator=(const GameObject& gameObject) = delete;
			GameObject& operator=(GameObject&& gameObject) = delete;

			inline unsigned int GetGameObjectID() const { return m_GameObjectID; };
			inline String GetName() const { return m_Name; };

			PLUGIN_API Components::TransformComponent& GetTransform();
			PLUGIN_API Scene* GetScene();

			template<typename T> PLUGIN_API T* GetComponent();
			template<typename T> PLUGIN_API std::vector<T*> GetComponents();

			template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<Components::Component, T>>>
			PLUGIN_API T* CreateComponent(Args&&... args);
			PLUGIN_API Components::Component* CreateComponent(size_t hash);

			PLUGIN_API void AddComponent(Components::Component* pComponent);
			PLUGIN_API void RemoveComponent(Components::Component* pComponent);

			void FixedUpdate();
			void Update();
			void LateUpdate();
			void Render(Renderer& renderer) const;

			// Events
			void SceneSwitched(Scene* pOldScene);
			void TransformChanged();

		protected:
			static unsigned int nextGameObjectID;
			const unsigned int m_GameObjectID;

			String m_Name;

			std::vector<Components::Component*> m_pComponents;
			Scene* m_pScene;

			Components::TransformComponent* m_pTransformComponent;
			Components::RigidBodyComponent* m_pRigidBodyComponent;
			Components::CharacterControllerComponent* m_pCharacterControllerComponent;

			friend class Scene;
			friend class SteffEngine::Core::Components::TransformComponent;
			friend class SteffEngine::Editor::EditorSceneStorage;
		};

		template<typename T>
		PLUGIN_API
		T* GameObject::GetComponent()
		{
			for (Components::Component* pComponent : m_pComponents)
			{
				if (pComponent->IsSameType<T>())
					return static_cast<T*>(pComponent);
			}

			return nullptr;
		}

		template<typename T>
		PLUGIN_API
		std::vector<T*> GameObject::GetComponents()
		{
			std::vector<T*> pComponents{};

			for (Components::Component* pComponent : m_pComponents)
			{
				if (pComponent->IsSameType<T>())
					pComponents.push_back(static_cast<T*>(pComponent));
			}

			return pComponents;
		}

		template<typename T, typename ...Args, typename>
		PLUGIN_API
		T* GameObject::CreateComponent(Args&&... args)
		{
			T* pComponent{ new T{ std::forward<Args>(args)... } };
			AddComponent(pComponent);

			return pComponent;
		}
	}
}