#include "pch.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "PhysxManager.h"
#include "BoxColliderComponent.h"
#include <extensions\PxRigidBodyExt.h>

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION(RigidBodyComponent);

RigidBodyComponent::RigidBodyComponent(bool isStatic, float density)
	: m_IsStatic(isStatic)
	, m_Density(density)
	, m_pRigidActor(nullptr)
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	physx::PxScene* pPhysicsScene{ m_pGameObject->GetScene()->GetPhysicsScene() };
	if (m_pRigidActor != nullptr && m_pRigidActor->getScene() == pPhysicsScene)
		pPhysicsScene->removeActor(*m_pRigidActor);

	SAFE_DELETE_PHYSX(m_pRigidActor);
}

void RigidBodyComponent::Initialize()
{
	std::vector<ColliderComponent*> pColliderComponents{ m_pGameObject->GetComponents<ColliderComponent>() };
	if (!pColliderComponents.empty())
		CreateActor(pColliderComponents);
}

void RigidBodyComponent::FixedUpdate()
{
	if (!m_IsStatic && m_pRigidActor != nullptr && !static_cast<physx::PxRigidDynamic*>(m_pRigidActor)->isSleeping())
	{
		m_pGameObject->GetTransform().SetPosition(PxVec3ToXMFLOAT3(m_pRigidActor->getGlobalPose().p), false);
		m_pGameObject->GetTransform().SetRotation(QuaternionToEuler(m_pRigidActor->getGlobalPose().q), false);
	}
}

void RigidBodyComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_IsStatic")
		ReinitializeActor();
	else if (variableName == "m_Density")
		SetMass(m_Density);
}

void RigidBodyComponent::ReinitializeActor()
{
	physx::PxScene* pPhysicsScene{ m_pGameObject->GetScene()->GetPhysicsScene() };
	if (m_pRigidActor != nullptr && m_pRigidActor->getScene() == pPhysicsScene)
		pPhysicsScene->removeActor(*m_pRigidActor);

	SAFE_DELETE_PHYSX(m_pRigidActor);

	std::vector<ColliderComponent*> pColliderComponents{ m_pGameObject->GetComponents<ColliderComponent>() };
	if (!pColliderComponents.empty())
		CreateActor(pColliderComponents);
}

void RigidBodyComponent::AddShape(ColliderComponent* pColliderComponent)
{
	if (m_pRigidActor == nullptr)
	{
		CreateActor({ pColliderComponent });
	}
	else
	{
		if (!m_pRigidActor->attachShape(*pColliderComponent->GetShape()))
		{
			Logging::Log(LogType::ERROR, "Something went wrong while adding a shape to a rigid actor!");
		}
	}
}

void RigidBodyComponent::RemoveShape(ColliderComponent* pColliderComponent)
{
	if (m_pRigidActor != nullptr)
		m_pRigidActor->detachShape(*pColliderComponent->GetShape(), true);
}

void RigidBodyComponent::CreateActor(const std::vector<ColliderComponent*>& pColliderComponents)
{
	physx::PxPhysics* pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	if (m_IsStatic)
	{
		m_pRigidActor = physx::PxCreateStatic(*pPhysics, physx::PxTransform{ XMFLOAT3ToPxVec3(m_pGameObject->GetTransform().GetPosition()), EulerToQuaternion(m_pGameObject->GetTransform().GetRotation()) },
			*pColliderComponents[0]->GetShape());
	}
	else
	{
		m_pRigidActor = physx::PxCreateDynamic(*pPhysics, physx::PxTransform{ XMFLOAT3ToPxVec3(m_pGameObject->GetTransform().GetPosition()), EulerToQuaternion(m_pGameObject->GetTransform().GetRotation()) },
			*pColliderComponents[0]->GetShape(), m_Density);
	}

	for (std::vector<ColliderComponent*>::const_iterator it{ pColliderComponents.cbegin() + 1 }; it != pColliderComponents.cend(); ++it)
	{
		AddShape(*it);
	}

	m_pRigidActor->userData = this;

	Scene* pScene{ m_pGameObject->GetScene() };
	if (pScene != nullptr)
		pScene->GetPhysicsScene()->addActor(*m_pRigidActor);
}

void RigidBodyComponent::SetMass(float mass)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to change the mass of a nullptr rigid actor!");
		return;
	}

	if (m_IsStatic)
	{
		Logging::Log(LogType::WARNING, "Tried to change the mass of a static rigidbody!");
		return;
	}

	m_Density = mass;
	physx::PxRigidBodyExt::updateMassAndInertia(*static_cast<physx::PxRigidBody*>(m_pRigidActor), mass, nullptr);
}

void RigidBodyComponent::SetMass(float mass, const physx::PxVec3& massCenterPose)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to change the mass of a nullptr rigid actor!");
		return;
	}

	if (m_IsStatic)
	{
		Logging::Log(LogType::WARNING, "Tried to change the mass of a static rigidbody!");
		return;
	}

	m_Density = mass;
	physx::PxRigidBodyExt::updateMassAndInertia(*static_cast<physx::PxRigidBody*>(m_pRigidActor), mass, &massCenterPose);
}

void RigidBodyComponent::AddForce(const physx::PxVec3& force, physx::PxForceMode::Enum forceMode)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to add force to a nullptr rigid actor!");
		return;
	}

	if (m_IsStatic)
	{
		Logging::Log(LogType::WARNING, "Tried to add a force to a static rigid actor!");
		return;
	}

	static_cast<physx::PxRigidBody*>(m_pRigidActor)->addForce(force, forceMode);
}

void RigidBodyComponent::AddTorque(const physx::PxVec3& torque, physx::PxForceMode::Enum torqueMode)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to add torque to a nullptr rigid actor!");
		return;
	}

	if (m_IsStatic)
	{
		Logging::Log(LogType::WARNING, "Tried to add a torque to a static rigid actor!");
		return;
	}

	static_cast<physx::PxRigidBody*>(m_pRigidActor)->addTorque(torque, torqueMode);
}

void RigidBodyComponent::ClearForce(physx::PxForceMode::Enum forceMode)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to clear force of a nullptr rigid actor!");
		return;
	}

	if (m_IsStatic)
	{
		Logging::Log(LogType::WARNING, "Tried to clear force of a static rigid actor!");
		return;
	}

	static_cast<physx::PxRigidBody*>(m_pRigidActor)->clearForce(forceMode);
}

void RigidBodyComponent::ClearTorque(physx::PxForceMode::Enum torqueMode)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to clear torque of a nullptr rigid actor!");
		return;
	}

	if (m_IsStatic)
	{
		Logging::Log(LogType::WARNING, "Tried to clear torque of a static rigid actor!");
		return;
	}

	static_cast<physx::PxRigidBody*>(m_pRigidActor)->clearTorque(torqueMode);
}

void RigidBodyComponent::SetName(const std::string& name)
{
	if (m_pRigidActor == nullptr)
	{
		Logging::Log(LogType::WARNING, "Tried to change rigid actors name but rigid actor is nullptr!");
		return;
	}

	m_pRigidActor->setName(name.c_str());
}

void RigidBodyComponent::UpdatePosition()
{
	if (m_pRigidActor == nullptr)
		return;

	m_pRigidActor->setGlobalPose(physx::PxTransform{ XMFLOAT3ToPxVec3(m_pGameObject->GetTransform().GetPosition()), m_pRigidActor->getGlobalPose().q });
}

void RigidBodyComponent::UpdateRotation()
{
	if (m_pRigidActor == nullptr)
		return;

	m_pRigidActor->setGlobalPose(physx::PxTransform{ m_pRigidActor->getGlobalPose().p, EulerToQuaternion(m_pGameObject->GetTransform().GetRotation()) });
}

void RigidBodyComponent::SceneSwitched(Scene* pOldScene)
{
	if (m_pRigidActor == nullptr)
		return;

	if (pOldScene != nullptr && m_pRigidActor->getScene() == pOldScene->GetPhysicsScene())
		pOldScene->GetPhysicsScene()->removeActor(*m_pRigidActor);

	if (m_pGameObject->GetScene() != nullptr)
		m_pGameObject->GetScene()->GetPhysicsScene()->addActor(*m_pRigidActor);
}