#pragma once
#include "Ray.h"
#include "VertexTypes.h"
#include "HitRecord.h"

namespace SteffEngine
{
	namespace Physics
	{
		namespace Raycasting
		{
			PLUGIN_API extern bool Raycast(HitRecord& hitRecord, float screenX, float screenY, float maxRayLength = FLT_MAX);
			PLUGIN_API extern bool Raycast(HitRecord& hitRecord, const XMFLOAT2& screenPosition, float maxRayLength = FLT_MAX);
			
			extern bool TriangleIntersection(HitRecord& hitRecord, const Ray& ray, const PosTexVertex& v0, const PosTexVertex& v1, const PosTexVertex& v2);
		}
	}
}