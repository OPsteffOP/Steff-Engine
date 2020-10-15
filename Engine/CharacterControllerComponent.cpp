#include "pch.h"
#include "CharacterControllerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "PhysxManager.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION(CharacterControllerComponent);

CharacterControllerComponent::CharacterControllerComponent(float radius, float height, float staticFriction, float dynamicFriction, float restitution, float slopeLimit, float stepOffset, float contactOffset, float density)
	: m_Radius(radius)
	, m_Height(height)
	, m_SlopeLimit(slopeLimit)
	, m_StepOffset(stepOffset)
	, m_ContactOffset(contactOffset)
	, m_Density(density)
	, m_StaticFriction(staticFriction)
	, m_DynamicFriction(dynamicFriction)
	, m_Restitution(restitution)
	, m_pController(nullptr)
{
	
}

CharacterControllerComponent::~CharacterControllerComponent()
{
	SAFE_DELETE_PHYSX(m_pMaterial);
	SAFE_DELETE_PHYSX(m_pController);
}

void CharacterControllerComponent::Initialize()
{
	CreateController();
}

void CharacterControllerComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_Radius")
	{
		SAFE_DELETE_PHYSX(m_pController);
		CreateController();
	}
	else if (variableName == "m_Height")
		m_pController->resize(m_Height);
	else if (variableName == "m_SlopeLimit")
		m_pController->setSlopeLimit(m_SlopeLimit);
	else if (variableName == "m_StepOffset")
		m_pController->setStepOffset(m_StepOffset);
	else if (variableName == "m_ContactOffset")
		m_pController->setContactOffset(m_ContactOffset);
	else if (variableName == "m_Density")
		physx::PxRigidBodyExt::updateMassAndInertia(*m_pController->getActor(), m_Density, nullptr);
	else if (variableName == "m_StaticFriction")
	{
		SAFE_DELETE_PHYSX(m_pMaterial);
		SAFE_DELETE_PHYSX(m_pController);
		CreateController();
	}
}

void CharacterControllerComponent::SceneSwitched(Scene* pOldScene)
{
	SAFE_DELETE_PHYSX(m_pController);
	CreateController();
}

void CharacterControllerComponent::CreateController()
{
	if (m_pMaterial == nullptr)
		m_pMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(m_StaticFriction, m_DynamicFriction, m_Restitution);

	// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/CharacterControllers.html
	if (m_pGameObject->GetScene() != nullptr)
	{
		physx::PxCapsuleControllerDesc capsuleControllerDesc{};
		capsuleControllerDesc.radius = m_Radius;
		capsuleControllerDesc.height = m_Height;
		capsuleControllerDesc.slopeLimit = cos(DegreesToRadians(m_SlopeLimit));
		capsuleControllerDesc.stepOffset = m_StepOffset;
		capsuleControllerDesc.contactOffset = m_ContactOffset;
		capsuleControllerDesc.position = XMFLOAT3ToPxExtendedVec3(m_pGameObject->GetTransform().GetPosition()); // possibly: make an offset to offset the collider (so it's not just the gameobject pos)
		capsuleControllerDesc.upDirection = XMFLOAT3ToPxVec3(m_pGameObject->GetTransform().GetUp());
		capsuleControllerDesc.material = m_pMaterial;
		capsuleControllerDesc.density = m_Density;
		// max jump height?
		capsuleControllerDesc.userData = this;

		m_pController = m_pGameObject->GetScene()->GetControllerManager()->createController(capsuleControllerDesc);
	}
}

void CharacterControllerComponent::Update()
{
	if (m_pController != nullptr)
	{
		m_pGameObject->GetTransform().SetPosition(PxExtendedVec3ToXMFLOAT3(m_pController->getPosition()));
		//m_pGameObject->GetTransform().SetRotation(m_pController->) // TODO: CHANGE ROTATION BASED ON THE UP VECTOR
	}
}

void CharacterControllerComponent::UpdatePosition()
{
	if (m_pController != nullptr)
		m_pController->setPosition(XMFLOAT3ToPxExtendedVec3(m_pGameObject->GetTransform().GetPosition()));
}

void CharacterControllerComponent::UpdateRotation()
{
	if (m_pController != nullptr)
		m_pController->setUpDirection(XMFLOAT3ToPxVec3(m_pGameObject->GetTransform().GetUp()));
}