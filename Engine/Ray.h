#pragma once
namespace SteffEngine
{
	namespace Physics
	{
		namespace Raycasting
		{
			struct Ray
			{
				explicit Ray(const XMVECTOR& origin, const XMVECTOR& direction, float tMax = FLT_MAX, float tMin = 0.0000001f);

				const XMVECTOR origin;
				const XMVECTOR direction;
				const float tMax;
				const float tMin;
			};
		}
	}
}