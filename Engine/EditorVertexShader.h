#pragma once
#include "VertexShader.h"
#include "ConstantBufferStatic.h"
#include "ConstantBufferDynamic.h"
#include "Texture.h"

namespace SteffEngine
{
	namespace Core
	{
		class EditorVertexShader : public VertexShader
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
			explicit EditorVertexShader(const std::wstring& compiledShaderPath);

			PLUGIN_API virtual void SetWorldMatrix(const XMFLOAT4X4& worldMatrix) override;
			PLUGIN_API virtual void SetViewProjectionMatrix(const XMFLOAT4X4& viewProjectionMatrix) override;

			void AddTexture(const Texture& texture);

		protected:
			virtual void Bind() const override;

		private:
			ConstantBufferStatic<Mesh_ConstantBuffer> m_MeshConstantBuffer;
			ConstantBufferDynamic<Camera_ConstantBuffer> m_CameraConstantBuffer;

			std::vector<ID3D11ShaderResourceView*> m_pResourceViews;
		};
	}
}