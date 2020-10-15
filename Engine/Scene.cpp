#include "pch.h"
#include "Scene.h"
#include "PostProcessingEffect.h"
#include "RenderTarget.h"
#include "MeshData.h"
#include "PhysxManager.h"
#include "PhysxSimulationEventCallback.h"
#include "FrameTimer.h"
#include "EditorSceneStorage.h"
#include <filesystem>

using namespace SteffEngine::Core;
using namespace SteffEngine::Editor;

Scene::Scene()
{
	InitializePhysx();
	InitializePostProcessing();
}

void Scene::InitializePhysx()
{
	// Physx scene
	physx::PxPhysics* pPhysx{ PhysxManager::GetInstance()->GetPhysics() };
	physx::PxSceneDesc sceneDesc{ pPhysx->getTolerancesScale() };
	sceneDesc.gravity = physx::PxVec3{ 0.f, -9.81f, 0.f };
	sceneDesc.simulationEventCallback = new PhysxSimulationEventCallback{};
	sceneDesc.cpuDispatcher = PhysxManager::GetInstance()->GetCpuDispatcher();
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.userData = this;

	m_pPhysicsScene = pPhysx->createScene(sceneDesc);
	if (m_pPhysicsScene == nullptr)
	{
		Logging::Log(LogType::ERROR, "Failed to create physics scene");
		return;
	}

	// Physx controller manager
	m_pControllerManager = PxCreateControllerManager(*m_pPhysicsScene);

#ifdef _DEBUG
	// Debug visualizations
	m_pPhysicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
	m_pPhysicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	m_pPhysicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
	m_pPhysicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
#endif
}

void Scene::InitializePostProcessing()
{
	std::vector<PosTexVertex> vertices
	{
		PosTexVertex{ XMFLOAT3{ -1.f, -1.f, 0.f }, XMFLOAT2{ 0.f, 1.f } },
		PosTexVertex{ XMFLOAT3{ 1.f, -1.f, 0.f }, XMFLOAT2{ 1.f, 1.f } },
		PosTexVertex{ XMFLOAT3{ 1.f, 1.f, 0.f }, XMFLOAT2{ 1.f, 0.f } },
		PosTexVertex{ XMFLOAT3{ -1.f, 1.f, 0.f }, XMFLOAT2{ 0.f, 0.f } }
	};

	std::vector<uint32_t> indices{ 0, 3, 1, 1, 3, 2 };

	m_pPostProcessingMesh = new MeshData{ vertices, indices };
}

Scene::~Scene()
{
	std::for_each(m_pGameObjects.begin(), m_pGameObjects.end(), [](GameObject* pGameObject) { SAFE_DELETE(pGameObject); });
	std::for_each(m_pPostProcessingStack.begin(), m_pPostProcessingStack.end(), [](PostProcessingEffect* pPostProcessingEffect) { SAFE_DELETE(pPostProcessingEffect); });

	SAFE_DELETE(m_pPostProcessingMesh);

	SAFE_DELETE_PHYSX(m_pControllerManager);
	SAFE_DELETE_CONST(m_pPhysicsScene->getSimulationEventCallback());
	SAFE_DELETE_PHYSX(m_pPhysicsScene);
}

PLUGIN_API
GameObject* Scene::CreateGameObject(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
{
	return CreateGameObject(DEFAULT_GAMEOBJECT_NAME, position, rotation, scale);
}

PLUGIN_API
GameObject* Scene::CreateGameObject(String name, const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
{
	GameObject* pGameObject{ new GameObject{ name, position, rotation, scale } };
	pGameObject->m_pScene = this;
	m_pGameObjects.push_back(pGameObject);

	pGameObject->SceneSwitched(nullptr);

	EditorSceneStorage::AddGameObject(pGameObject);

	return pGameObject;
}

PLUGIN_API
void Scene::AddGameObject(GameObject* pGameObject)
{
	if (pGameObject->m_pScene != nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to add a GameObject to a scene while it's already added to another scene!");
		return;
	}

	pGameObject->m_pScene = this;
	m_pGameObjects.push_back(pGameObject);

	pGameObject->SceneSwitched(nullptr);

	EditorSceneStorage::AddGameObject(pGameObject);
}

PLUGIN_API 
void Scene::RemoveGameObject(GameObject* pGameObject, bool shouldDelete)
{
	EditorSceneStorage::RemoveGameObject(pGameObject);

	if (pGameObject->m_pScene != this)
	{
		Logging::Log(LogType::WARNING, "Tried to remove a GameObject from a scene it's not added to");
		return;
	}

	Scene* pOldScene{ pGameObject->m_pScene };

	pGameObject->m_pScene = nullptr;
	m_pGameObjects.erase(std::remove(m_pGameObjects.begin(), m_pGameObjects.end(), pGameObject));

	pGameObject->SceneSwitched(pOldScene);
	
	if (shouldDelete)
		SAFE_DELETE(pGameObject);
}

PLUGIN_API
void Scene::AddPostProcessingEffect(PostProcessingEffect* pPostProcessingEffect)
{
	m_pPostProcessingStack.push_back(pPostProcessingEffect);
}

PLUGIN_API
void Scene::RemovePostProcessingEffect(PostProcessingEffect* pPostProcessingEffect, bool shouldDelete)
{
	m_pPostProcessingStack.erase(std::remove(m_pPostProcessingStack.begin(), m_pPostProcessingStack.end(), pPostProcessingEffect));

	if (shouldDelete)
		SAFE_DELETE(pPostProcessingEffect);
}

void Scene::FixedUpdate()
{
	std::for_each(m_pGameObjects.begin(), m_pGameObjects.end(), [](GameObject* pGameObject) { pGameObject->FixedUpdate(); });
	
	m_pPhysicsScene->simulate(SteffEngine::Timers::FrameTimer::GetInstance()->GetElapsedSec()); // TODO: MAKE THIS FIXED ELAPSED SEC (fixed update currently isn't actually on fixed times)
	m_pPhysicsScene->fetchResults(true);

	// Physx visual debugging
	if (DEBUG_TOOLS_ENABLED)
	{
		const physx::PxRenderBuffer& debugRenderBuffer = m_pPhysicsScene->getRenderBuffer();
		auto test = debugRenderBuffer.getNbLines();
		for (physx::PxU32 i{ 0 }; i < debugRenderBuffer.getNbLines(); ++i)
		{
			const physx::PxDebugLine& line = debugRenderBuffer.getLines()[i];
			Debugging::DrawLine(PxVec3ToXMFLOAT3(line.pos0), PxVec3ToXMFLOAT3(line.pos1));
		}
	}
}

void Scene::Update()
{
	std::for_each(m_pGameObjects.begin(), m_pGameObjects.end(), [](GameObject* pGameObject) { pGameObject->Update(); });
}

void Scene::LateUpdate()
{
	std::for_each(m_pGameObjects.begin(), m_pGameObjects.end(), [](GameObject* pGameObject) { pGameObject->LateUpdate(); });
}

void Scene::Render(Renderer& renderer) const
{
	// Render the scene
	std::for_each(m_pGameObjects.begin(), m_pGameObjects.end(), [&renderer](GameObject* pGameObject) { pGameObject->Render(renderer); });

	// Post-processing effects
	if (!m_pPostProcessingStack.empty())
	{
		ID3D11ShaderResourceView* pCurrentRenderState{};
		for (PostProcessingEffect* pPostProcessingEffect : m_pPostProcessingStack)
		{
			pCurrentRenderState = renderer.GetCurrentRenderTarget()->GetColorShaderResourceView();

			pPostProcessingEffect->Prepare(renderer, pCurrentRenderState);
			pPostProcessingEffect->RootBind();

			m_pPostProcessingMesh->BindVertexBuffer();
			m_pPostProcessingMesh->BindIndexBuffer();

			renderer.DrawIndexed(unsigned int(m_pPostProcessingMesh->indices.size()));
		}

		ID3D11ShaderResourceView* resetShaderResourceViews[] = { nullptr };
		DeviceManager::m_pDeviceContext->PSSetShaderResources(0, 1, resetShaderResourceViews); // reset shader resources views

		// copy the content of the final render target to the backbuffer
		const RenderTarget* finalResultRenderTarget{ renderer.GetCurrentRenderTarget() };
		renderer.SetCurrentRenderTarget(nullptr); // reset the render target back to the default one
		DeviceManager::m_pDeviceContext->CopyResource(renderer.GetCurrentRenderTarget()->GetColorBuffer(), finalResultRenderTarget->GetColorBuffer());
	}
}

PLUGIN_API
physx::PxScene* Scene::GetPhysicsScene() const
{
	return m_pPhysicsScene;
}

PLUGIN_API
physx::PxControllerManager* Scene::GetControllerManager() const
{
	return m_pControllerManager;
}

PLUGIN_API
std::vector<GameObject*>& Scene::GetGameObjects()
{
	return m_pGameObjects;
}