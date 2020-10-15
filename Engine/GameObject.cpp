#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "RigidBodyComponent.h"
#include "CharacterControllerComponent.h"
#include "EditorSceneStorage.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Editor;

unsigned int GameObject::nextGameObjectID{ 0 };

PLUGIN_API
GameObject::GameObject(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	: GameObject(DEFAULT_GAMEOBJECT_NAME, position, rotation, scale)
{

}

PLUGIN_API
GameObject::GameObject(String name, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	: m_GameObjectID(nextGameObjectID)
	, m_Name(name)
	, m_pTransformComponent(nullptr)
	, m_pScene(nullptr)
	, m_pRigidBodyComponent(nullptr)
	, m_pCharacterControllerComponent(nullptr)
{
	++nextGameObjectID;

	m_pTransformComponent = CreateComponent<TransformComponent>(position, rotation, scale);
	m_pTransformComponent->m_pGameObject = this;
}

GameObject::~GameObject()
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [this](Component* pComponent) { EditorSceneStorage::RemoveComponent(this, pComponent); delete pComponent; });
}

PLUGIN_API
TransformComponent& GameObject::GetTransform()
{
	return *m_pTransformComponent;
}

PLUGIN_API
Scene* GameObject::GetScene()
{
	return m_pScene;
}

PLUGIN_API
void GameObject::AddComponent(Component* pComponent)
{
	pComponent->m_pGameObject = this;
	pComponent->Initialize();
	pComponent->PostInitialize();
	m_pComponents.push_back(pComponent);

	if (pComponent->IsSameType<RigidBodyComponent>())
		m_pRigidBodyComponent = static_cast<RigidBodyComponent*>(pComponent);
	else if (pComponent->IsSameType<CharacterControllerComponent>())
		m_pCharacterControllerComponent = static_cast<CharacterControllerComponent*>(pComponent);

	// Adding to the editor access list
	EditorSceneStorage::AddComponent(this, pComponent);
}

PLUGIN_API
void GameObject::RemoveComponent(Component* pComponent)
{
	// Removing from the editor access list
	EditorSceneStorage::RemoveComponent(this, pComponent);

	if (pComponent->IsSameType<RigidBodyComponent>())
		m_pRigidBodyComponent = nullptr;
	else if (pComponent->IsSameType<CharacterControllerComponent>())
		m_pCharacterControllerComponent = nullptr;

	std::vector<Component*>::const_iterator foundComponentIterator{ std::find(m_pComponents.cbegin(), m_pComponents.cend(), pComponent) };
	if (foundComponentIterator != m_pComponents.cend())
	{
		delete* foundComponentIterator;
		m_pComponents.erase(foundComponentIterator);
	}
}

void GameObject::FixedUpdate()
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [](Component* pComponent) 
		{ 
			pComponent->FixedUpdate();
		});
}

void GameObject::Update()
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [](Component* pComponent)
		{
			pComponent->Update();
		});
}

void GameObject::LateUpdate()
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [](Component* pComponent)
		{
			pComponent->LateUpdate();
		});
}

void GameObject::Render(Renderer& renderer) const
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [&renderer](Component* pComponent)
		{
			pComponent->Render(renderer);
		});
}

void GameObject::SceneSwitched(Scene* pOldScene)
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [pOldScene](Component* pComponent)
		{
			pComponent->SceneSwitched(pOldScene);
		});
}

void GameObject::TransformChanged()
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [](Component* pComponent)
		{
			pComponent->TransformChanged();
		});
}