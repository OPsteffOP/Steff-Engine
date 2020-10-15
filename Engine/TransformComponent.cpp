#include "pch.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "CharacterControllerComponent.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

#define WORLD_FORWARD_VECTOR XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define WORLD_UP_VECTOR XMVectorSet(0.f, 1.f, 0.f, 0.f)

COMPONENT_DEFINITION(TransformComponent);

PLUGIN_API
TransformComponent::TransformComponent(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	: m_Position(position)
	, m_Rotation(rotation)
	, m_RotationRadians(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z))
	, m_Scale(scale)
{
	UpdateWorldMatrix();
}

void TransformComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_Position")
	{
		// Update RigidBodyComponent if exists
		if (m_pGameObject->m_pRigidBodyComponent != nullptr)
			m_pGameObject->m_pRigidBodyComponent->UpdatePosition();

		// Update CharacterControllerComponent if exists
		if (m_pGameObject->m_pCharacterControllerComponent != nullptr)
			m_pGameObject->m_pCharacterControllerComponent->UpdatePosition();
	}
	else if (variableName == "m_Rotation")
	{
		m_RotationRadians.x = XMConvertToRadians(m_Rotation.x);
		m_RotationRadians.y = XMConvertToRadians(m_Rotation.y);
		m_RotationRadians.z = XMConvertToRadians(m_Rotation.z);

		// Update RigidBodyComponent if exists
		if (m_pGameObject->m_pRigidBodyComponent != nullptr)
			m_pGameObject->m_pRigidBodyComponent->UpdateRotation();

		// Update CharacterControllerComponent if exists
		if (m_pGameObject->m_pCharacterControllerComponent != nullptr)
			m_pGameObject->m_pCharacterControllerComponent->UpdateRotation();
	}

	UpdateWorldMatrix();
	m_pGameObject->TransformChanged();
}

PLUGIN_API
void TransformComponent::Move(float dX, float dY, float dZ, Axis axis)
{
	if (axis == Axis::LOCAL)
	{
		const XMMATRIX rotationAlongForwardMatrix{ XMMatrixRotationRollPitchYaw(m_RotationRadians.x, m_RotationRadians.y, m_RotationRadians.z) };
		const XMVECTOR movementVector{ XMVectorSet(dX, dY, dZ, 0.f) };

		XMFLOAT3 correctDeltaMovement{};
		XMStoreFloat3(&correctDeltaMovement, XMVector3TransformCoord(movementVector, rotationAlongForwardMatrix));

		m_Position.x += correctDeltaMovement.x;
		m_Position.y += correctDeltaMovement.y;
		m_Position.z += correctDeltaMovement.z;
	}
	else if(axis == Axis::WORLD)
	{
		m_Position.x += dX;
		m_Position.y += dY;
		m_Position.z += dZ;
	}

	// Update RigidBodyComponent if exists
	if (m_pGameObject->m_pRigidBodyComponent != nullptr)
		m_pGameObject->m_pRigidBodyComponent->UpdatePosition();

	// Update CharacterControllerComponent if exists
	if (m_pGameObject->m_pCharacterControllerComponent != nullptr)
		m_pGameObject->m_pCharacterControllerComponent->UpdatePosition();

	UpdateWorldMatrix();
	m_pGameObject->TransformChanged();
}

PLUGIN_API
void TransformComponent::Move(const XMFLOAT3& dPosition, Axis axis)
{
	Move(dPosition.x, dPosition.y, dPosition.z);
}

void TransformComponent::SetPosition(const XMFLOAT3& position, bool shouldUpdateRigidBody)
{
	m_Position = position;

	if (shouldUpdateRigidBody)
	{
		// Update RigidBodyComponent if exists
		if (m_pGameObject->m_pRigidBodyComponent != nullptr)
			m_pGameObject->m_pRigidBodyComponent->UpdatePosition();

		// Update CharacterControllerComponent if exists
		if (m_pGameObject->m_pCharacterControllerComponent != nullptr)
			m_pGameObject->m_pCharacterControllerComponent->UpdatePosition();
	}

	UpdateWorldMatrix();
	m_pGameObject->TransformChanged();
}

PLUGIN_API
void TransformComponent::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3{ x, y, z });
}

PLUGIN_API
void TransformComponent::SetPosition(const XMFLOAT3& position)
{
	SetPosition(position, true);
}

PLUGIN_API
void TransformComponent::Rotate(float dYawDegree, float dPitchDegree, float dRollDegree)
{
	m_Rotation.x += dYawDegree;
	m_Rotation.y += dPitchDegree;
	m_Rotation.z += dRollDegree;

	m_RotationRadians.x += XMConvertToRadians(dYawDegree);
	m_RotationRadians.y += XMConvertToRadians(dPitchDegree);
	m_RotationRadians.z += XMConvertToRadians(dRollDegree);

	// Update RigidBodyComponent if exists
	if (m_pGameObject->m_pRigidBodyComponent != nullptr)
		m_pGameObject->m_pRigidBodyComponent->UpdateRotation();

	// Update CharacterControllerComponent if exists
	if (m_pGameObject->m_pCharacterControllerComponent != nullptr)
		m_pGameObject->m_pCharacterControllerComponent->UpdateRotation();

	UpdateWorldMatrix();
	m_pGameObject->TransformChanged();
}

PLUGIN_API
void TransformComponent::Rotate(const XMFLOAT3& dRotationDegree)
{
	Rotate(dRotationDegree.x, dRotationDegree.y, dRotationDegree.z);
}

void TransformComponent::SetRotation(const XMFLOAT3& rotation, bool shouldUpdateRigidBody)
{
	m_Rotation = rotation;
	m_RotationRadians.x = XMConvertToRadians(rotation.x);
	m_RotationRadians.y = XMConvertToRadians(rotation.y);
	m_RotationRadians.z = XMConvertToRadians(rotation.z);

	if (shouldUpdateRigidBody)
	{
		// Update RigidBodyComponent if exists
		if (m_pGameObject->m_pRigidBodyComponent != nullptr)
			m_pGameObject->m_pRigidBodyComponent->UpdateRotation();

		// Update CharacterControllerComponent if exists
		if (m_pGameObject->m_pCharacterControllerComponent != nullptr)
			m_pGameObject->m_pCharacterControllerComponent->UpdateRotation();
	}

	UpdateWorldMatrix();
	m_pGameObject->TransformChanged();
}

PLUGIN_API
void TransformComponent::SetRotation(float yawDegree, float pitchDegree, float rollDegree)
{
	SetRotation(XMFLOAT3{ yawDegree, pitchDegree, rollDegree });
}

PLUGIN_API
void TransformComponent::SetRotation(const XMFLOAT3& rotationDegree)
{
	SetRotation(rotationDegree, true);
}

PLUGIN_API
void TransformComponent::Scale(float dX, float dY, float dZ)
{
	SetScale(m_Scale.x + dX, m_Scale.y + dY, m_Scale.z + dZ);
}

PLUGIN_API
void TransformComponent::Scale(const XMFLOAT3& dScale)
{
	Scale(dScale.x, dScale.y, dScale.z);
}

PLUGIN_API
void TransformComponent::SetScale(float x, float y, float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;

	UpdateWorldMatrix();
	m_pGameObject->TransformChanged();
}

PLUGIN_API
void TransformComponent::SetScale(const XMFLOAT3& scale)
{
	SetScale(scale.x, scale.y, scale.z);
}

PLUGIN_API
const XMFLOAT4X4& TransformComponent::GetWorldMatrix() const
{
	return m_WorldMatrix;
}

void TransformComponent::UpdateWorldMatrix()
{
	// World matrix
	const XMMATRIX worldTranslateMatrix{ XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z) };
	const XMMATRIX worldRotateMatrix{ XMMatrixRotationRollPitchYaw(m_RotationRadians.x, m_RotationRadians.y, m_RotationRadians.z) };
	const XMMATRIX worldScaleMatrix{ XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z) };

	XMStoreFloat4x4(&m_WorldMatrix, worldTranslateMatrix * worldRotateMatrix * worldScaleMatrix);

	// Forward, up and right vector
	const XMVECTOR forward{ XMVector4Transform(WORLD_FORWARD_VECTOR, worldRotateMatrix) };
	const XMVECTOR right{ XMVector3Normalize(XMVector3Cross(WORLD_UP_VECTOR, forward)) };

	XMStoreFloat3(&m_Forward, forward);
	XMStoreFloat3(&m_Right, right);
	XMStoreFloat3(&m_Up, XMVector3Normalize(XMVector3Cross(forward, right)));
}

PLUGIN_API
const XMFLOAT3& TransformComponent::GetUp() const
{
	return m_Up;
}

PLUGIN_API
const XMFLOAT3& TransformComponent::GetRight() const
{
	return m_Right;
}

PLUGIN_API
const XMFLOAT3& TransformComponent::GetForward() const
{
	return m_Forward;
}