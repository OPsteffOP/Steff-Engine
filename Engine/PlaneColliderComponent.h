#pragma once
#include "ColliderComponent.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			class PlaneColliderComponent : public ColliderComponent
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit PlaneColliderComponent(float staticFriction, float dynamicFriction, float restitution, bool isTrigger = false);

				virtual void Initialize() override;
				virtual void VariableChanged(const std::string_view& variableName) override;

			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				PlaneColliderComponent()
					: ColliderComponent(physx::PxGeometryType::ePLANE, false, 0.f, 0.f, 0.f)
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
			};

			COMPONENT_REGISTRATION(PlaneColliderComponent);
		}
	}
}