#include "pch.h"
#include "PlaneColliderComponent.h"
#include "PhysxManager.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION_INHERITANCE(PlaneColliderComponent, ColliderComponent);

PLUGIN_API
PlaneColliderComponent::PlaneColliderComponent(float staticFriction, float dynamicFriction, float restitution, bool isTrigger)
	: ColliderComponent(physx::PxGeometryType::ePLANE, isTrigger, staticFriction, dynamicFriction, restitution)
{

}

void PlaneColliderComponent::Initialize()
{
	physx::PxPhysics* pPhysics{ PhysxManager::GetInstance()->GetPhysics() };

	const physx::PxPlaneGeometry planeGeometry{};
	m_pShape = pPhysics->createShape(planeGeometry, *GetMaterial(), true, GetShapeFlags());
}

void PlaneColliderComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_IsTrigger")
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