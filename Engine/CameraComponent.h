#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include <optional>

namespace SteffEngine
{
	namespace Core
	{
		class CameraManager;

		namespace Components
		{
			class CameraComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit CameraComponent(float fovAngle = 90.f);

				virtual void Initialize() override;

				virtual void VariableChanged(const std::string_view& variableName) override;
				virtual void TransformChanged();

				PLUGIN_API const XMFLOAT3& GetPosition() const;
				PLUGIN_API const XMFLOAT3& GetRotation() const;

				PLUGIN_API void LookAt(float x, float y, float z);
				PLUGIN_API void LookAt(const XMFLOAT3& focusPoint);

				PLUGIN_API void TrackPoint(float x, float y, float z);
				PLUGIN_API void TrackPoint(const XMFLOAT3& focusPoint);
				PLUGIN_API void StopTracking();

				PLUGIN_API void Activate();

				float GetFOVScaleFactor() const;

				XMFLOAT4X4 GetViewProjectionMatrix();
				XMFLOAT4X4 GetInverseViewProjectionMatrix();

			private:
				void UpdateProjectionMatrix();
				void UpdateViewMatrix();

				EDITOR_READWRITE bool m_IsActive;

				EDITOR_READWRITE float m_FovDegrees;
				float m_FovRadians;
				EDITOR_READWRITE float m_NearPlane;
				EDITOR_READWRITE float m_FarPlane;

				std::optional<XMFLOAT3> m_FocusPoint;

				XMFLOAT4X4 m_ProjectionMatrix;
				XMFLOAT4X4 m_ViewMatrix;

				friend class CameraManager;
			};

			COMPONENT_REGISTRATION(CameraComponent);
		}
	}
}