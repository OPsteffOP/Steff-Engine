#include "pch.h"
#include "Ray.h"

using namespace SteffEngine::Physics::Raycasting;

Ray::Ray(const XMVECTOR& origin, const XMVECTOR& direction, float tMax, float tMin)
	: origin(origin)
	, direction(direction)
	, tMax(tMax)
	, tMin(tMin)
{

}