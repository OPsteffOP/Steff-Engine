#pragma once
namespace SteffEngine
{
	namespace Core
	{
		class GameObject;
	}

	namespace Physics
	{
		namespace Raycasting
		{
			struct HitRecord
			{
				PLUGIN_API HitRecord()
					: t(FLT_MAX)
					, intersectionPoint(XMFLOAT3{ 0.f, 0.f, 0.f })
					, pGameObject(nullptr)
				{

				}

				float t;
				XMFLOAT3 intersectionPoint;
				SteffEngine::Core::GameObject* pGameObject;
			};
		}
	}
}