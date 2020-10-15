#pragma once
#include "VertexShader.h"
#include "ConstantBufferStatic.h"
#include "ConstantBufferDynamic.h"

namespace SteffEngine
{
	namespace Core
	{
		class PosVertexShader : public VertexShader
		{
			struct Mesh_ConstantBuffer
			{
				XMFLOAT4X4 worldMatrix;
			};

			struct Camera_ConstantBuffer
			{
				XMFLOAT4X4 viewProjectionMatrix;
			};

		public:
			PLUGIN_API explicit PosVertexShader(const std::wstring& compiledShaderPath);

			PLUGIN_API virtual void SetWorldMatrix(const XMFLOAT4X4& worldMatrix) override;
			PLUGIN_API virtual void SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) override;

		protected:
			virtual void Bind() const override;

		private:
			ConstantBufferStatic<Mesh_ConstantBuffer> m_MeshConstantBuffer;
			ConstantBufferDynamic<Camera_ConstantBuffer> m_CameraConstantBuffer;
		};
	}
}