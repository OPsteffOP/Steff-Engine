#include "pch.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "GameObject.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;

COMPONENT_DEFINITION(CameraComponent);

#define WORLD_FORWARD_VECTOR XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define WORLD_UP_VECTOR XMVectorSet(0.f, 1.f, 0.f, 0.f)

PLUGIN_API
CameraComponent::CameraComponent(float fovAngle)
	: m_FovDegrees(fovAngle)
	, m_FovRadians(XMConvertToRadians(fovAngle))
	, m_NearPlane(0.1f)
	, m_FarPlane(500.f)
	, m_ProjectionMatrix()
	, m_ViewMatrix()
{

}

void CameraComponent::Initialize()
{
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void CameraComponent::VariableChanged(const std::string_view& variableName)
{
	if (variableName == "m_FovDegrees")
	{
		m_FovRadians = XMConvertToRadians(m_FovDegrees);
		UpdateProjectionMatrix();
	}
	else if (variableName == "m_NearPlane" || variableName == "m_FarPlane")
	{
		UpdateProjectionMatrix();
	}
	else if (variableName == "m_IsActive")
	{
		if (m_IsActive)
			Activate();
		else
			m_IsActive = true;
	}
}

void CameraComponent::TransformChanged()
{
	UpdateViewMatrix();
}

PLUGIN_API const XMFLOAT3& CameraComponent::GetPosition() const
{
	return m_pGameObject->GetTransform().GetPosition();
}

PLUGIN_API const XMFLOAT3& CameraComponent::GetRotation() const
{
	return m_pGameObject->GetTransform().GetRotationRadians();
}

PLUGIN_API
void CameraComponent::LookAt(float x, float y, float z)
{
	LookAt(XMFLOAT3{ x, y, z });
}

PLUGIN_API
void CameraComponent::LookAt(const XMFLOAT3& focusPoint)
{
	const XMFLOAT3 forward{ Tools::GetVector(m_pGameObject->GetTransform().GetPosition(), focusPoint) };

	m_pGameObject->GetTransform().Rotate(
		asin(-forward.y),
		atan2(forward.x, forward.z),
		0.f
	);

	UpdateViewMatrix();
}

PLUGIN_API
void CameraComponent::TrackPoint(float x, float y, float z)
{
	TrackPoint(XMFLOAT3{ x, y, z });
}

PLUGIN_API
void CameraComponent::TrackPoint(const XMFLOAT3& focusPoint)
{
	m_FocusPoint.emplace(focusPoint);
}

PLUGIN_API
void CameraComponent::StopTracking()
{
	m_FocusPoint = std::nullopt;
}

void CameraComponent::UpdateProjectionMatrix()
{
	const float aspectRatio{ float(WINDOW_WIDTH) / float(WINDOW_HEIGHT) };
	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixPerspectiveFovLH(m_FovRadians, aspectRatio, m_NearPlane, m_FarPlane));
}

void CameraComponent::UpdateViewMatrix()
{
	const XMMATRIX rotationMatrix{ XMMatrixRotationRollPitchYaw(m_pGameObject->GetTransform().GetRotationRadians().x, 
		m_pGameObject->GetTransform().GetRotationRadians().y, m_pGameObject->GetTransform().GetRotationRadians().z) };
	const XMVECTOR forward{ XMVector4Transform(WORLD_FORWARD_VECTOR, rotationMatrix) };

	const XMVECTOR right{ XMVector3Normalize(XMVector3Cross(WORLD_UP_VECTOR, forward)) };
	const XMVECTOR up{ XMVector3Normalize(XMVector3Cross(forward, right)) };

	const XMVECTOR position{ XMVectorSet(m_pGameObject->GetTransform().GetPosition().x, 
		m_pGameObject->GetTransform().GetPosition().y, m_pGameObject->GetTransform().GetPosition().z, 1.f) };

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixInverse(nullptr, XMMATRIX{ right, up, forward, position })); // view matrix = from world space to camera space
}

PLUGIN_API
void CameraComponent::Activate()
{
	CameraManager::GetInstance()->SetActiveCamera(this);
}

float CameraComponent::GetFOVScaleFactor() const
{
	return tanf(m_FovRadians / 2.f);
}

XMFLOAT4X4 CameraComponent::GetViewProjectionMatrix()
{
	if (m_FocusPoint.has_value())
	{
		LookAt(m_FocusPoint.value());
	}

	XMMATRIX viewMatrix{ XMLoadFloat4x4(&m_ViewMatrix) };
	XMMATRIX projectionMatrix{ XMLoadFloat4x4(&m_ProjectionMatrix) };

	XMFLOAT4X4 viewProjectionMatrix{};
	XMStoreFloat4x4(&viewProjectionMatrix, XMMatrixInverse(nullptr, viewMatrix * projectionMatrix));

	return viewProjectionMatrix;
}

XMFLOAT4X4 CameraComponent::GetInverseViewProjectionMatrix()
{
	XMMATRIX viewMatrix{ XMLoadFloat4x4(&m_ViewMatrix) };
	XMMATRIX projectionMatrix{ XMLoadFloat4x4(&m_ProjectionMatrix) };

	XMFLOAT4X4 inverseViewProjectionMatrix{};
	XMStoreFloat4x4(&inverseViewProjectionMatrix, viewMatrix * projectionMatrix);

	return inverseViewProjectionMatrix;
}