#pragma once
#include "Component.h"

namespace SteffEngine
{
	namespace Core
	{
		class GameObject;

		namespace Components
		{
			enum class Axis
			{
				LOCAL,
				WORLD
			};

			EDITOR_HIDDEN class TransformComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit TransformComponent(const XMFLOAT3& position, const XMFLOAT3& rotation = { 0.f, 0.f, 0.f }, const XMFLOAT3& scale = { 1.f, 1.f, 1.f });

				virtual void VariableChanged(const std::string_view& variableName) override;

				PLUGIN_API inline const XMFLOAT3& GetPosition() const { return m_Position; };
				PLUGIN_API inline const XMFLOAT3& GetRotation() const { return m_Rotation; };
				PLUGIN_API inline const XMFLOAT3& GetRotationRadians() const { return m_RotationRadians; };
				PLUGIN_API inline const XMFLOAT3& GetScale() const { return m_Scale; };

				PLUGIN_API void Move(float dX, float dY, float dZ, Axis axis = Axis::LOCAL);
				PLUGIN_API void Move(const XMFLOAT3& dPosition, Axis axis = Axis::LOCAL);

				PLUGIN_API void SetPosition(float x, float y, float z);
				PLUGIN_API void SetPosition(const XMFLOAT3& position);

				PLUGIN_API void Rotate(float dYawDegree, float dPitchDegree, float dRollDegree);
				PLUGIN_API void Rotate(const XMFLOAT3& dRotationDegree);

				PLUGIN_API void SetRotation(float yawDegree, float pitchDegree, float rollDegree);
				PLUGIN_API void SetRotation(const XMFLOAT3& rotationDegree);

				PLUGIN_API void Scale(float dX, float dY, float dZ);
				PLUGIN_API void Scale(const XMFLOAT3& dScale);

				PLUGIN_API void SetScale(float x, float y, float z);
				PLUGIN_API void SetScale(const XMFLOAT3& scale);

				PLUGIN_API const XMFLOAT4X4& GetWorldMatrix() const;
				PLUGIN_API const XMFLOAT3& GetUp() const;
				PLUGIN_API const XMFLOAT3& GetRight() const;
				PLUGIN_API const XMFLOAT3& GetForward() const;

			private:
				void UpdateWorldMatrix();
				void SetPosition(const XMFLOAT3& position, bool shouldUpdateRigidBody);
				void SetRotation(const XMFLOAT3& position, bool shouldUpdateRigidBody);

				GameObject* m_pGameObject;

				EDITOR_READWRITE XMFLOAT3 m_Position;
				EDITOR_READWRITE XMFLOAT3 m_Rotation;
				XMFLOAT3 m_RotationRadians;
				EDITOR_READWRITE XMFLOAT3 m_Scale;

				XMFLOAT4X4 m_WorldMatrix;

				XMFLOAT3 m_Up;
				XMFLOAT3 m_Right;
				XMFLOAT3 m_Forward;

				friend class GameObject;
				friend class RigidBodyComponent;
			};

			COMPONENT_REGISTRATION(TransformComponent);
		}
	}
}