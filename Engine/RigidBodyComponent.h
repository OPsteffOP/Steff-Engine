#pragma once
#include "Component.h"

namespace SteffEngine
{
	namespace Core
	{
		namespace Components
		{
			class ColliderComponent;

			class RigidBodyComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit RigidBodyComponent(bool isStatic = false, float density = 1.f);
				virtual ~RigidBodyComponent() override;

				virtual void Initialize() override;
				virtual void FixedUpdate() override;

				virtual void VariableChanged(const std::string_view& variableName) override;

				void AddShape(ColliderComponent* pColliderComponent);
				void RemoveShape(ColliderComponent* pColliderComponent);

				PLUGIN_API void SetMass(float mass);
				PLUGIN_API void SetMass(float mass, const physx::PxVec3& massCenterPose);

				PLUGIN_API void AddForce(const physx::PxVec3& force, physx::PxForceMode::Enum forceMode);
				PLUGIN_API void AddTorque(const physx::PxVec3& torque, physx::PxForceMode::Enum torqueMode);

				PLUGIN_API void ClearForce(physx::PxForceMode::Enum);
				PLUGIN_API void ClearTorque(physx::PxForceMode::Enum);

				PLUGIN_API void SetName(const std::string& name);

				void UpdatePosition();
				void UpdateRotation();

				virtual void SceneSwitched(Scene* pOldScene) override;

			private:
				void CreateActor(const std::vector<ColliderComponent*>& pColliderComponents);
				void ReinitializeActor();

				EDITOR_READWRITE bool m_IsStatic;
				EDITOR_READWRITE float m_Density;

				physx::PxRigidActor* m_pRigidActor;
			};

			COMPONENT_REGISTRATION(RigidBodyComponent);
		}
	}
}