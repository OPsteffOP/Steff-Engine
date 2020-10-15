#pragma once
#include "VertexShader.h"
#include "ConstantBufferStatic.h"
#include "ConstantBufferDynamic.h"

namespace SteffEngine
{
	namespace Core
	{
		class TimePosTexVertexShader : public VertexShader
		{
			struct Mesh_ConstantBuffer
			{
				XMFLOAT4X4 worldMatrix;
			};

			struct Camera_ConstantBuffer
			{
				XMFLOAT4X4 viewProjectionMatrix;
				float time;
				XMFLOAT3 padding;
			};

		public:
			PLUGIN_API explicit TimePosTexVertexShader(const std::wstring& compiledShaderPath);

			PLUGIN_API virtual void SetWorldMatrix(const XMFLOAT4X4& worldMatrix) override;
			PLUGIN_API virtual void SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) override;

			PLUGIN_API virtual void SetTime(float time);

		protected:
			virtual void Bind() const override;

		private:
			ConstantBufferStatic<Mesh_ConstantBuffer> m_MeshConstantBuffer;
			ConstantBufferDynamic<Camera_ConstantBuffer> m_CameraConstantBuffer;
		};
	}
}