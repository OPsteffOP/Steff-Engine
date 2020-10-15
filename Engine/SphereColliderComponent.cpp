#include "pch.h"
#include "SphereColliderComponent.h"
#include "PhysxManager.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION_INHERITANCE(SphereColliderComponent, ColliderComponent);

PLUGIN_API
SphereColliderComponent::SphereColliderComponent(float radius, float staticFriction, float dynamicFriction, float restitution, bool isTrigger)
	: ColliderComponent(physx::PxGeometryType::eSPHERE, isTrigger, staticFriction, dynamicFriction, restitution)
	, m_Radius(radius)
{

}

void SphereColliderComponent::Initialize()
{
	physx::PxPhysics* pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	const physx::PxSphereGeometry sphereGeometry{ m_Radius };
	m_pShape = pPhysics->createShape(sphereGeometry, *GetMaterial(), true, GetShapeFlags());
}

void SphereColliderComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_Radius")
	{
		const physx::PxSphereGeometry sphereGeometry{ m_Radius };
		m_pShape->setGeometry(sphereGeometry);
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