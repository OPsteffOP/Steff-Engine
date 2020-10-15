#pragma once
#include "Component.h"

namespace SteffEngine
{
	namespace Core
	{
		class Scene;

		namespace Components
		{
			class CharacterControllerComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit CharacterControllerComponent(float radius, float height, float staticFriction, float dynamicFriction, float restitution, float slopeLimit, float stepOffset, float contactOffset = 0.1f, float density = 1.f);
				virtual ~CharacterControllerComponent() override;

				virtual void Initialize() override;
				virtual void VariableChanged(const std::string_view& variableName) override;
				virtual void SceneSwitched(Scene* pOldScene) override;

				virtual void Update() override;

				void UpdatePosition();
				void UpdateRotation();

				// TODO: change size functions
				// TODO: move function
				// TODO: gravity in the fixedupdate

			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				CharacterControllerComponent()
					: m_Radius(10.f)
					, m_Height(10.f)
					, m_SlopeLimit(45.f)
					, m_StepOffset(0.5f)
					, m_ContactOffset(0.1f)
					, m_Density(1.f)
					, m_StaticFriction(0.f)
					, m_DynamicFriction(0.f)
					, m_Restitution(0.f)
					, m_pController(nullptr)
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				void CreateController();

				EDITOR_READWRITE float m_Radius;
				EDITOR_READWRITE float m_Height;
				EDITOR_READWRITE float m_SlopeLimit;
				EDITOR_READWRITE float m_StepOffset;
				EDITOR_READWRITE float m_ContactOffset;
				EDITOR_READWRITE float m_Density;

				EDITOR_READWRITE float m_StaticFriction;
				EDITOR_READWRITE float m_DynamicFriction;
				EDITOR_READWRITE float m_Restitution;

				physx::PxMaterial* m_pMaterial;
				physx::PxController* m_pController;
			};

			COMPONENT_REGISTRATION(CharacterControllerComponent);
		}
	}
}