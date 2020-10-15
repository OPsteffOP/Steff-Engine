#pragma once
namespace SteffEngine
{
	namespace Physics
	{
		PLUGIN_API inline physx::PxVec2 XMFLOAT2ToPxVec2(const XMFLOAT2& data);
		PLUGIN_API inline physx::PxVec3 XMFLOAT3ToPxVec3(const XMFLOAT3& data);
		PLUGIN_API inline physx::PxExtendedVec3 XMFLOAT3ToPxExtendedVec3(const XMFLOAT3& data);

		PLUGIN_API inline XMFLOAT2 PxVec2ToXMFLOAT2(const physx::PxVec2& data);
		PLUGIN_API inline XMFLOAT3 PxVec3ToXMFLOAT3(const physx::PxVec3& data);
		PLUGIN_API inline XMFLOAT3 PxExtendedVec3ToXMFLOAT3(const physx::PxExtendedVec3& data);

		PLUGIN_API inline physx::PxQuat EulerToQuaternion(const XMFLOAT3& data);
		PLUGIN_API inline XMFLOAT3 QuaternionToEuler(const physx::PxQuat& data);
	}
}