#pragma once
#include "Component.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			EDITOR_HIDDEN class ColliderComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit ColliderComponent(physx::PxGeometryType::Enum geometryType, bool isTrigger, float staticFriction, float dynamicFriction, float restitution);
				virtual ~ColliderComponent() override;

				virtual void PostInitialize() override;

				void UpdateMaterial();

				physx::PxMaterial* GetMaterial();
				physx::PxShapeFlags GetShapeFlags() const;
				physx::PxShape* GetShape() const;

			protected:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				ColliderComponent()
					: m_GeometryType(physx::PxGeometryType::eINVALID)
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				const physx::PxGeometryType::Enum m_GeometryType;
				EDITOR_READWRITE bool m_IsTrigger;

				EDITOR_READWRITE float m_StaticFriction;
				EDITOR_READWRITE float m_DynamicFriction;
				EDITOR_READWRITE float m_Restitution;

				physx::PxMaterial* m_pMaterial;
				physx::PxShape* m_pShape;
			};

			COMPONENT_REGISTRATION(ColliderComponent);
		}
	}
}