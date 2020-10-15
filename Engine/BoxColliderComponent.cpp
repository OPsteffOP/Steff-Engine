#include "pch.h"
#include "BoxColliderComponent.h"
#include "PhysxManager.h"
#include "RigidBodyComponent.h"
#include "GameObject.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION_INHERITANCE(BoxColliderComponent, ColliderComponent);

BoxColliderComponent::BoxColliderComponent(float width, float height, float depth, float staticFriction, float dynamicFriction, float restitution, bool isTrigger)
	: BoxColliderComponent(XMFLOAT3{ width, height, depth }, staticFriction, dynamicFriction, restitution, isTrigger)
{
	
}

BoxColliderComponent::BoxColliderComponent(const XMFLOAT3& size, float staticFriction, float dynamicFriction, float restitution, bool isTrigger)
	: ColliderComponent(physx::PxGeometryType::Enum::eBOX, isTrigger, staticFriction, dynamicFriction, restitution)
	, m_Size(size)
{

}

void BoxColliderComponent::Initialize()
{
	physx::PxPhysics* pPhysics{ PhysxManager::GetInstance()->GetPhysics() };
	
	const physx::PxBoxGeometry boxGeometry{ m_Size.x / 2.f, m_Size.y / 2.f, m_Size.z / 2.f };
	m_pShape = pPhysics->createShape(boxGeometry, *GetMaterial(), true, GetShapeFlags());
}

void BoxColliderComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_Size")
	{
		const physx::PxBoxGeometry boxGeometry{ m_Size.x / 2.f, m_Size.y / 2.f, m_Size.z / 2.f };
		m_pShape->setGeometry(boxGeometry);
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

void BoxColliderComponent::SetSize(const XMFLOAT3& size)
{
	m_Size = size;

	const physx::PxBoxGeometry boxGeometry{ m_Size.x / 2.f, m_Size.y / 2.f, m_Size.z / 2.f };
	m_pShape->setGeometry(boxGeometry);
}