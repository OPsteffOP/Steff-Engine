#include "pch.h"
#include "SceneManager.h"

using namespace SteffEngine::Core;

SceneManager* SceneManager::m_pInstance{ nullptr };

PLUGIN_API
SceneManager* SceneManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new SceneManager{};
	}

	return m_pInstance;
}

SceneManager::SceneManager()
	: m_pActiveScene(nullptr)
{

}

void SceneManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

SceneManager::~SceneManager()
{
	std::for_each(m_pScenes.begin(), m_pScenes.end(), 
		[](const std::pair<std::string, Scene*>& pPair) { SAFE_DELETE_CONST(pPair.second); });
}

PLUGIN_API
Scene* SceneManager::CreateScene(const std::string& sceneName)
{
	std::unordered_map<std::string, Scene*>::iterator sceneIterator{ m_pScenes.find(sceneName) };
	if (sceneIterator == m_pScenes.cend())
	{
		sceneIterator = m_pScenes.emplace(sceneName, new Scene{}).first;
	}

	return sceneIterator->second;
}

PLUGIN_API
Scene* SceneManager::GetScene(const std::string& sceneName)
{
	const std::unordered_map<std::string, Scene*>::iterator sceneIterator{ m_pScenes.find(sceneName) };
	if (sceneIterator == m_pScenes.cend())
	{
		return nullptr;
	}

	return sceneIterator->second;
}

PLUGIN_API
void SceneManager::SetActiveScene(const std::string& sceneName)
{
	const std::unordered_map<std::string, Scene*>::iterator sceneIterator{ m_pScenes.find(sceneName) };
	if (sceneIterator == m_pScenes.cend())
	{
		m_pActiveScene = nullptr;
		return;
	}

	m_pActiveScene = sceneIterator->second;

	// Update the GameObjects for the editor
	Editor::EditorSceneStorage::ClearGameObjects();
	for (GameObject* pGameObject : m_pActiveScene->GetGameObjects())
	{
		Editor::EditorSceneStorage::AddGameObject(pGameObject);
	}
}

PLUGIN_API
Scene* SceneManager::GetActiveScene()
{
	return m_pActiveScene;
}

void SceneManager::FixedUpdate()
{
	if (m_pActiveScene != nullptr)
	{
		m_pActiveScene->FixedUpdate();
	}
}

void SceneManager::Update()
{
	if (m_pActiveScene != nullptr)
	{
		m_pActiveScene->Update();
	}
}

void SceneManager::LateUpdate()
{
	if (m_pActiveScene != nullptr)
	{
		m_pActiveScene->LateUpdate();
	}
}

void SceneManager::Render(Renderer& renderer)
{
	if (m_pActiveScene != nullptr)
	{
		m_pActiveScene->Render(renderer);
	}
}