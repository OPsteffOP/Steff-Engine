#pragma once
#include "Component.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			class MeshRendererComponent;

			class WaterSplashComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				WaterSplashComponent();

				virtual void Initialize() override;
				virtual void Update() override;

			private:
				void HandleRayCollision(const Physics::Raycasting::HitRecord& hitRecord);
				void UpdateSplashSize();

				const MeshRendererComponent* m_pMeshRendererComponent;
				const XMFLOAT2 m_SplashSizeIncrease;
				const XMFLOAT2 m_SplashMaxSize;
				XMFLOAT2 m_SplashCurrentSize;
			};
		}
	}
}