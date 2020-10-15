#include "pch.h"
#include "PhysxManager.h"
#include "Scene.h"

using namespace SteffEngine::Core;

PhysxManager* PhysxManager::m_pInstance{ nullptr };

PhysxManager* PhysxManager::GetInstance()
{
	if (m_pInstance == nullptr)
		m_pInstance = new PhysxManager{};

	return m_pInstance;
}

void PhysxManager::Destroy()
{
	SAFE_DELETE(m_pInstance);
}

PhysxManager::~PhysxManager()
{
	SAFE_DELETE(m_pCpuDispatcher);

	if (m_pPvd != nullptr)
		m_pPvd->disconnect();

	SAFE_DELETE_PHYSX(m_pPhysx);
	SAFE_DELETE_PHYSX(m_pPvd);
	SAFE_DELETE_PHYSX(m_pPvdTransport);
	SAFE_DELETE_PHYSX(m_pPhysxFoundation);
}

void PhysxManager::Initialize()
{
	m_pPhysxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PhysxAllocator, m_PhysxErrorCallback);
	if (m_pPhysxFoundation == nullptr)
	{
		Logging::Log(LogType::ERROR, "Failed to initialize physx (PxCreateFoundation)!");
		return;
	}

#ifdef _DEBUG
	bool recordMemoryAllocations = true;

	// Enable debugging and profiling (Physx visual debugger)
	m_pPvd = PxCreatePvd(*m_pPhysxFoundation);
	m_pPvdTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 100);
	m_pPvd->connect(*m_pPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
#else
	bool recordMemoryAllocations = false;

	// Disable debugging and profiling (Physx visual debugger)
	m_pPvd = nullptr;
#endif

	m_pPhysx = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pPhysxFoundation, physx::PxTolerancesScale{}, recordMemoryAllocations, m_pPvd);
	if (m_pPhysx == nullptr)
	{
		Logging::Log(LogType::ERROR, "Failed to initialize physx (PxCreatePhysics)!");
		return;
	}

	m_pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1); // TODO: Is 1 thread the most optimized way? - also check if gpu would be more performant
}

physx::PxPhysics* PhysxManager::GetPhysics() const
{
	return m_pPhysx;
}

physx::PxCpuDispatcher* PhysxManager::GetCpuDispatcher() const
{
	return m_pCpuDispatcher;
}