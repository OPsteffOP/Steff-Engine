#pragma once
#include "ColliderComponent.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			class BoxColliderComponent : public ColliderComponent
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit BoxColliderComponent(float width, float height, float depth, float staticFriction, float dynamicFriction, float restitution, bool isTrigger = false);
				PLUGIN_API explicit BoxColliderComponent(const XMFLOAT3& size, float staticFriction, float dynamicFriction, float restitution, bool isTrigger = false);

				virtual void Initialize() override;
				virtual void VariableChanged(const std::string_view& variableName) override;

				void SetSize(const XMFLOAT3& size);

			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				BoxColliderComponent()
					: ColliderComponent(physx::PxGeometryType::eBOX, false, 0.f, 0.f, 0.f)
					, m_Size(10.f, 10.f, 10.f)
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				EDITOR_READWRITE XMFLOAT3 m_Size;
			};

			COMPONENT_REGISTRATION(BoxColliderComponent);
		}
	}
}