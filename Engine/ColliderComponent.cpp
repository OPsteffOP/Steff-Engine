#include "pch.h"
#include "ColliderComponent.h"
#include "PhysxManager.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION(ColliderComponent);

ColliderComponent::ColliderComponent(physx::PxGeometryType::Enum geometryType, bool isTrigger, float staticFriction, float dynamicFriction, float restitution)
	: m_GeometryType(geometryType)
	, m_IsTrigger(isTrigger)
	, m_StaticFriction(staticFriction)
	, m_DynamicFriction(dynamicFriction)
	, m_Restitution(restitution)
	, m_pShape(nullptr)
	, m_pMaterial(nullptr)
{

}

ColliderComponent::~ColliderComponent()
{
	SAFE_DELETE_PHYSX(m_pMaterial);
	SAFE_DELETE_PHYSX(m_pShape);
}

void ColliderComponent::PostInitialize()
{
	// TODO: QUERY AND SIMULATION DATA!

	RigidBodyComponent* pRigidBodyComponent{ m_pGameObject->GetComponent<RigidBodyComponent>() };
	if (pRigidBodyComponent != nullptr)
		pRigidBodyComponent->AddShape(this);
}

void ColliderComponent::UpdateMaterial()
{
	SAFE_DELETE_PHYSX(m_pMaterial);
	m_pMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(m_StaticFriction, m_DynamicFriction, m_Restitution);
}

physx::PxMaterial* ColliderComponent::GetMaterial()
{
	if (m_pMaterial == nullptr)
		UpdateMaterial();

	return m_pMaterial;
}

physx::PxShapeFlags ColliderComponent::GetShapeFlags() const
{
	return (m_IsTrigger) ? physx::PxShapeFlag::eTRIGGER_SHAPE : physx::PxShapeFlag::eSIMULATION_SHAPE | physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eVISUALIZATION;
}

physx::PxShape* ColliderComponent::GetShape() const
{
	return m_pShape;
}