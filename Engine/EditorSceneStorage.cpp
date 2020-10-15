#include "pch.h"
#include "EditorSceneStorage.h"
#include "Component.h"
#include "GameObject.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Editor;

std::unordered_map<unsigned int, Component*> EditorSceneStorage::m_pComponents{};
std::unordered_map<SteffEngine::Core::Components::Component*, std::vector<ExposedVariableData>> EditorSceneStorage::m_pComponentsData{};
std::unordered_map<unsigned int, GameObject*> EditorSceneStorage::m_pGameObjects{};
std::vector<GameObjectData> EditorSceneStorage::m_GameObjectsData{};
std::unordered_map<unsigned int, std::vector<unsigned int>> EditorSceneStorage::m_GameObjectComponentsLink{};

// Components
Component* EditorSceneStorage::GetComponent(unsigned int id)
{
	std::unordered_map<unsigned int, Component*>::iterator foundIterator{ m_pComponents.find(id) };
	if (foundIterator != m_pComponents.end())
		return foundIterator->second;

	return nullptr;
}

std::pair<size_t, const ExposedVariableData*> EditorSceneStorage::GetComponentData(unsigned int id)
{
	Component* pComponent{ GetComponent(id) };
	assert(pComponent != nullptr);

	return std::make_pair(m_pComponentsData[pComponent].size(), m_pComponentsData[pComponent].data());
}

void EditorSceneStorage::AddComponent(GameObject* pGameObject, Component* pComponent)
{
	m_pComponents.emplace(pComponent->GetComponentID(), pComponent);
	m_pComponentsData.emplace(pComponent, std::move(pComponent->GetEditorData()));
	m_GameObjectComponentsLink[pGameObject->GetGameObjectID()].push_back(pComponent->GetComponentID());
}

void EditorSceneStorage::RemoveComponent(GameObject* pGameObject, Component* pComponent)
{
	m_pComponents.erase(pComponent->GetComponentID());
	m_pComponentsData.erase(pComponent);
	m_GameObjectComponentsLink[pGameObject->GetGameObjectID()].erase(std::find(m_GameObjectComponentsLink[pGameObject->GetGameObjectID()].begin(),
		m_GameObjectComponentsLink[pGameObject->GetGameObjectID()].end(), pComponent->GetComponentID()));
}

void EditorSceneStorage::ClearComponents()
{
	m_pComponents.clear();
}

// GameObjects
const std::vector<GameObjectData>& EditorSceneStorage::GetGameObjectsData()
{
	return m_GameObjectsData;
}

GameObject* EditorSceneStorage::GetGameObject(unsigned int id)
{
	std::unordered_map<unsigned int, GameObject*>::iterator foundIterator{ m_pGameObjects.find(id) };
	if (foundIterator != m_pGameObjects.end())
		return foundIterator->second;

	return nullptr;
}

const std::vector<unsigned int>& EditorSceneStorage::GetGameObjectComponents(unsigned int id)
{
	return m_GameObjectComponentsLink[id];
}

void EditorSceneStorage::AddGameObject(GameObject* pGameObject)
{
	m_pGameObjects.emplace(pGameObject->GetGameObjectID(), pGameObject);
	m_GameObjectsData.emplace_back(pGameObject->GetName(), pGameObject->GetGameObjectID());
}

void EditorSceneStorage::RemoveGameObject(GameObject* pGameObject)
{
	m_pGameObjects.erase(pGameObject->GetGameObjectID());
	m_GameObjectsData.erase(std::find_if(m_GameObjectsData.begin(), m_GameObjectsData.end(),
		[pGameObject](const GameObjectData& data) { return data.id == pGameObject->GetGameObjectID(); }));
}

void EditorSceneStorage::ClearGameObjects()
{
	m_pGameObjects.clear();
}