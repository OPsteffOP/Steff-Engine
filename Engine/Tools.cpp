#include "pch.h"
#include "Tools.h"
#include "CameraManager.h"

#include <codecvt>
#include <locale>

using namespace SteffEngine::Core;

PLUGIN_API
inline XMFLOAT3 SteffEngine::Tools::GetVector(const XMFLOAT3& startPosition, const XMFLOAT3& endPosition)
{
	return XMFLOAT3{
		endPosition.x - startPosition.x,
		endPosition.y - startPosition.y,
		endPosition.z - startPosition.z,
	};
}

PLUGIN_API
inline XMFLOAT3 Tools::ScreenToWorld(const XMFLOAT2& screenPosition)
{
	const XMFLOAT2 NDCPosition{ 
		2.f * (screenPosition.x / WINDOW_WIDTH) - 1.f, 
		-2.f * (screenPosition.y / WINDOW_HEIGHT) + 1.f };

	const XMFLOAT4 homogeneousClipPosition{ NDCPosition.x, NDCPosition.y, -1.f, 1.f };

	const XMVECTOR homogeneousClipPositionVector{ XMLoadFloat4(&homogeneousClipPosition) };
	const XMMATRIX viewProjectionMatrix{ XMLoadFloat4x4(&CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix()) };
	const XMVECTOR worldPositionVector{ XMVector3TransformCoord(homogeneousClipPositionVector, viewProjectionMatrix) };

	XMFLOAT3 worldPosition{};
	XMStoreFloat3(&worldPosition, worldPositionVector);
	return worldPosition;
}

PLUGIN_API
inline bool Tools::FloatCompare(float flt1, float flt2, float epsilon)
{
	return abs(flt1 - flt2) <= epsilon;
}

PLUGIN_API
inline float Tools::DegreesToRadians(float degrees)
{
	return degrees * XM_PI / 180.f;
}

PLUGIN_API
inline float Tools::RadiansToDegrees(float radians)
{
	return radians * 180.f / XM_PI;
}

PLUGIN_API
inline std::wstring Tools::StringToWString(const std::string& str)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
#pragma warning(pop)
}

PLUGIN_API
inline std::string Tools::WStringToString(const std::wstring& str)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
#pragma warning(pop)
}

bool SteffEngine::Tools::operator==(const XMFLOAT4X4& matrix1, const XMFLOAT4X4& matrix2)
{
	return matrix1._11 == matrix2._11 &&
		matrix1._12 == matrix2._12 &&
		matrix1._13 == matrix2._13 &&
		matrix1._14 == matrix2._14 &&
		matrix1._21 == matrix2._21 &&
		matrix1._22 == matrix2._22 &&
		matrix1._23 == matrix2._23 &&
		matrix1._24 == matrix2._24 &&
		matrix1._31 == matrix2._31 &&
		matrix1._32 == matrix2._32 &&
		matrix1._33 == matrix2._33 &&
		matrix1._34 == matrix2._34 &&
		matrix1._41 == matrix2._41 &&
		matrix1._42 == matrix2._42 &&
		matrix1._43 == matrix2._43 &&
		matrix1._44 == matrix2._44;
}

XMFLOAT3 SteffEngine::Tools::operator+(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	return XMFLOAT3{ 
		pos1.x + pos2.x,
		pos1.y + pos2.y,
		pos1.z + pos2.z
	};
}

XMFLOAT3 SteffEngine::Tools::operator/(const XMFLOAT3& pos, float scalar)
{
	return XMFLOAT3{
		pos.x / scalar,
		pos.y / scalar,
		pos.z / scalar
	};
}