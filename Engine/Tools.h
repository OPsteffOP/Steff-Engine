#pragma once
#include <string>

namespace SteffEngine
{
	namespace Tools
	{
		PLUGIN_API inline XMFLOAT3 GetVector(const XMFLOAT3& startPosition, const XMFLOAT3& endPosition);

		PLUGIN_API inline XMFLOAT3 ScreenToWorld(const XMFLOAT2& screenPosition);

		PLUGIN_API inline bool FloatCompare(float flt1, float flt2, float epsilon = FLT_EPSILON);

		PLUGIN_API inline float DegreesToRadians(float degrees);
		PLUGIN_API inline float RadiansToDegrees(float radians);

		PLUGIN_API inline std::wstring StringToWString(const std::string& str);
		PLUGIN_API inline std::string WStringToString(const std::wstring& str);

		bool operator==(const XMFLOAT4X4& matrix1, const XMFLOAT4X4& matrix2);
		XMFLOAT3 operator+(const XMFLOAT3& pos1, const XMFLOAT3& pos2);
		XMFLOAT3 operator/(const XMFLOAT3& pos, float scalar);
	}
}