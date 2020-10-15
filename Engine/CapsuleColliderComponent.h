#pragma once
#include "ColliderComponent.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			class CapsuleColliderComponent : public ColliderComponent
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit CapsuleColliderComponent(float radius, float height, float staticFriction, float dynamicFriction, float restitution, bool isTrigger = false);

				virtual void Initialize() override;
				virtual void VariableChanged(const std::string_view& variableName) override;

			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				CapsuleColliderComponent()
					: ColliderComponent(physx::PxGeometryType::eCAPSULE, false, 0.f, 0.f, 0.f)
					, m_Height(10.f)
					, m_Radius(2.5f)
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				EDITOR_READWRITE float m_Radius;
				EDITOR_READWRITE float m_Height;
			};

			COMPONENT_REGISTRATION(CapsuleColliderComponent);
		}
	}
}