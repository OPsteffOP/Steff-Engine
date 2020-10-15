#include "pch.h"
#include "CapsuleColliderComponent.h"
#include "PhysxManager.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION_INHERITANCE(CapsuleColliderComponent, ColliderComponent);

PLUGIN_API
CapsuleColliderComponent::CapsuleColliderComponent(float radius, float height, float staticFriction, float dynamicFriction, float restitution, bool isTrigger)
	: ColliderComponent(physx::PxGeometryType::eCAPSULE, isTrigger, staticFriction, dynamicFriction, restitution)
	, m_Radius(radius)
	, m_Height(height)
{

}

void CapsuleColliderComponent::Initialize()
{
	physx::PxPhysics* pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	const physx::PxCapsuleGeometry capsuleGeometry{ m_Radius, m_Height / 2.f };
	m_pShape = pPhysics->createShape(capsuleGeometry, *GetMaterial(), true, GetShapeFlags());
}

void CapsuleColliderComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_Radius" || variableName == "m_Height")
	{
		const physx::PxCapsuleGeometry capsuleGeometry{ m_Radius, m_Height / 2.f };
		m_pShape->setGeometry(capsuleGeometry);
	}
	else if (variableName == "m_IsTrigger")
	{
		m_pShape->setFlags(GetShapeFlags());
	}
	else if (variableName == "m_StaticFriction" || variableName == "m_DynamicFriction" || variableName == "m_Restitution")
	{
		UpdateMaterial();

		physx::PxMaterial* pMaterial{ GetMaterial() };
		m_pShape->setMaterials(&pMaterial, 1);
	}
}