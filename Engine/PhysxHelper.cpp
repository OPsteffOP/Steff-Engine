#include "pch.h"
#include "PhysxHelper.h"

PLUGIN_API
inline physx::PxVec2 SteffEngine::Physics::XMFLOAT2ToPxVec2(const XMFLOAT2& data)
{
	return physx::PxVec2{ data.x, data.y };
}

PLUGIN_API
inline physx::PxVec3 SteffEngine::Physics::XMFLOAT3ToPxVec3(const XMFLOAT3& data)
{
	return physx::PxVec3{ data.x, data.y, data.z };
}

PLUGIN_API
inline physx::PxExtendedVec3 SteffEngine::Physics::XMFLOAT3ToPxExtendedVec3(const XMFLOAT3& data)
{
    return physx::PxExtendedVec3{ double(data.x), double(data.y), double(data.z) };
}

PLUGIN_API
inline XMFLOAT2 SteffEngine::Physics::PxVec2ToXMFLOAT2(const physx::PxVec2& data)
{
	return XMFLOAT2{ data.x, data.y };
}

PLUGIN_API
inline XMFLOAT3 SteffEngine::Physics::PxVec3ToXMFLOAT3(const physx::PxVec3& data)
{
	return XMFLOAT3{ data.x, data.y, data.z };
}

PLUGIN_API
inline XMFLOAT3 SteffEngine::Physics::PxExtendedVec3ToXMFLOAT3(const physx::PxExtendedVec3& data)
{
    return XMFLOAT3{ float(data.x), float(data.y), float(data.z) };
}

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_Code
PLUGIN_API
inline physx::PxQuat SteffEngine::Physics::EulerToQuaternion(const XMFLOAT3& data)
{
    float cy = cosf(DegreesToRadians(data.x) * 0.5f);
    float sy = sinf(DegreesToRadians(data.x) * 0.5f);
    float cp = cosf(DegreesToRadians(data.y) * 0.5f);
    float sp = sinf(DegreesToRadians(data.y) * 0.5f);
    float cr = cosf(DegreesToRadians(data.z) * 0.5f);
    float sr = sinf(DegreesToRadians(data.z) * 0.5f);

    return physx::PxQuat
    {
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    };
}

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_Code
PLUGIN_API
inline XMFLOAT3 SteffEngine::Physics::QuaternionToEuler(const physx::PxQuat& data)
{
    XMFLOAT3 euler{};

    // roll (x-axis rotation)
    float sinr_cosp = 2.f * (data.w * data.x + data.y * data.z);
    float cosr_cosp = 1.f - 2.f * (data.x * data.x + data.y * data.y);
    euler.z = RadiansToDegrees(std::atan2(sinr_cosp, cosr_cosp));

    // pitch (y-axis rotation)
    float sinp = 2.f * (data.w * data.y - data.z * data.x);
    if (std::abs(sinp) >= 1)
        euler.y = RadiansToDegrees(std::copysign(XM_PI / 2, sinp)); // use 90 degrees if out of range
    else
        euler.y = RadiansToDegrees(std::asin(sinp));

    // yaw (z-axis rotation)
    float siny_cosp = 2.f * (data.w * data.z + data.x * data.y);
    float cosy_cosp = 1.f - 2.f * (data.y * data.y + data.z * data.z);
    euler.x = RadiansToDegrees(std::atan2(siny_cosp, cosy_cosp));

    return euler;
}