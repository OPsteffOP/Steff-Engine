#pragma once
#include "ColliderComponent.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			class SphereColliderComponent : public ColliderComponent
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit SphereColliderComponent(float radius, float staticFriction, float dynamicFriction, float restitution, bool isTrigger = false);

				virtual void Initialize() override;
				virtual void VariableChanged(const std::string_view& variableName) override;
				
			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				SphereColliderComponent()
					: ColliderComponent(physx::PxGeometryType::eSPHERE, false, 0.f, 0.f, 0.f)
					, m_Radius(10.f)
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				EDITOR_READWRITE float m_Radius;
			};

			COMPONENT_REGISTRATION(SphereColliderComponent);
		}
	}
}