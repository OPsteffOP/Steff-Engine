#include "pch.h"
#include "Raycast.h"
#include "CameraManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRendererComponent.h"

using namespace SteffEngine::Core;
using namespace SteffEngine::Core::Components;
using namespace SteffEngine::Physics;

PLUGIN_API
inline bool Raycasting::Raycast(HitRecord& hitRecord, float screenX, float screenY, float maxRayLength)
{
	// Screen space to world space
	const CameraComponent* pCamera{ CameraManager::GetInstance()->GetActiveCamera() };
	const XMVECTOR cameraPositionWorldSpace{ XMLoadFloat3(&pCamera->GetPosition()) };
	const XMVECTOR viewPlaneWorldSpace{ XMLoadFloat3(&ScreenToWorld(XMFLOAT2{ screenX, screenY })) };

	bool didHit{ false };
	for (GameObject* pGameObject : SceneManager::GetInstance()->GetActiveScene()->GetGameObjects())
	{
		MeshRendererComponent* pMeshRendererComponent{ pGameObject->GetComponent<MeshRendererComponent>() };
		if (pMeshRendererComponent == nullptr)
			continue;

		// World space to mesh space
		const XMMATRIX meshSpaceMatrix{ XMMatrixInverse(nullptr, XMLoadFloat4x4(&pGameObject->GetTransform().GetWorldMatrix())) };

		const XMVECTOR cameraPositionMeshSpace{ XMVector3TransformCoord(cameraPositionWorldSpace, meshSpaceMatrix) };
		const XMVECTOR viewPlaneMeshSpace{ XMVector3TransformCoord(viewPlaneWorldSpace, meshSpaceMatrix) };

		const XMVECTOR rayDirectionMeshSpace{ XMVector3Normalize(viewPlaneMeshSpace - cameraPositionMeshSpace) };

		// Doing the raycast
		const Raycasting::Ray ray{ cameraPositionMeshSpace, rayDirectionMeshSpace, maxRayLength };
		HitRecord tempHitRecord{};
		tempHitRecord.pGameObject = pGameObject;
		if (pMeshRendererComponent->RayHit(ray, tempHitRecord))
		{
			didHit = true;
			if (tempHitRecord.t < hitRecord.t)
			{
				hitRecord = tempHitRecord;
				continue;
			}
		}
	}
	
	return didHit;
}

PLUGIN_API
inline bool Raycasting::Raycast(HitRecord& hitRecord, const XMFLOAT2& screenPosition, float maxRayLength)
{
	return Raycasting::Raycast(hitRecord, screenPosition.x, screenPosition.y, maxRayLength);
}

inline bool Raycasting::TriangleIntersection(HitRecord& hitRecord, const Ray& ray, const PosTexVertex& v0, const PosTexVertex& v1, const PosTexVertex& v2)
{
	const XMVECTOR edge1{ XMLoadFloat3(&GetVector(v0.position, v2.position)) };
	const XMVECTOR edge2{ XMLoadFloat3(&GetVector(v0.position, v1.position)) };

	const XMVECTOR normal{ XMVector3Normalize(XMVector3Cross(edge1, edge2)) };
	if (XMVector3Equal(XMVector3Dot(normal, ray.direction), XMVectorZero()))
	{
		return false;
	}

	const XMFLOAT3 center{ (v0.position + v1.position + v2.position) / 3.f };
	const XMVECTOR rayOriginToCenter{ XMLoadFloat3(&center) - ray.origin };

	const XMVECTOR t{ XMVector3Dot(XMLoadFloat3(&center) - ray.origin, normal) / XMVector3Dot(ray.direction, normal) };
	const XMVECTOR tMin{ XMVectorSet(ray.tMin, ray.tMin, ray.tMin, ray.tMin) };
	const XMVECTOR tMax{ XMVectorSet(ray.tMax, ray.tMax, ray.tMax, ray.tMax) };
	if (XMVector3Less(t, tMin) || XMVector3Greater(t, tMax))
	{
		return false;
	}

	const XMVECTOR intersectionPoint{ ray.origin + t * ray.direction };

	XMVECTOR edge{ edge2 };
	if (XMVector3Greater(XMVector3Dot(XMVector3Cross(XMLoadFloat3(&v0.position) - intersectionPoint, edge), normal), XMVectorZero()))
	{
		return false;
	}

	edge = XMLoadFloat3(&GetVector(v1.position, v2.position));
	if (XMVector3Greater(XMVector3Dot(XMVector3Cross(XMLoadFloat3(&v1.position) - intersectionPoint, edge), normal), XMVectorZero()))
	{
		return false;
	}

	edge = XMLoadFloat3(&GetVector(v2.position, v0.position));
	if (XMVector3Greater(XMVector3Dot(XMVector3Cross(XMLoadFloat3(&v2.position) - intersectionPoint, edge), normal), XMVectorZero()))
	{
		return false;
	}

	XMStoreFloat(&hitRecord.t, t);
	XMStoreFloat3(&hitRecord.intersectionPoint, XMVector3TransformCoord(intersectionPoint, XMLoadFloat4x4(&hitRecord.pGameObject->GetTransform().GetWorldMatrix())));

	return true;
}