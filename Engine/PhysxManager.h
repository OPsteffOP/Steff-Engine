#pragma once
#include "PhysxAllocator.h"
#include "PhysxErrorCallback.h"

namespace SteffEngine
{
	namespace Core
	{
		class Scene;
	}

	namespace Physics
	{
		class PhysxManager
		{
		public:
			static PhysxManager* GetInstance();
			static void Destroy();

			void Initialize();

			physx::PxPhysics* GetPhysics() const;
			physx::PxCpuDispatcher* GetCpuDispatcher() const;

		private:
			PhysxManager() = default;
			~PhysxManager();

			static PhysxManager* m_pInstance;

			PhysxAllocator m_PhysxAllocator;
			PhysxErrorCallback m_PhysxErrorCallback;

			physx::PxFoundation* m_pPhysxFoundation;
			physx::PxPhysics* m_pPhysx;

			physx::PxPvd* m_pPvd;
			physx::PxPvdTransport* m_pPvdTransport;

			physx::PxCpuDispatcher* m_pCpuDispatcher;
		};
	}
}