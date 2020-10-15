#pragma once
#include "Component.h"
#include "ConstantBufferDynamic.h"

namespace SteffEngine
{
	namespace Core
	{
		class MeshData;
		class Texture;

		class VertexShader;
		class PixelShader;
		class GeometryShader;

		namespace Components
		{
			class TransformComponent;

			class InstancedMeshRendererComponent : public Component
			{
				struct InstanceData
				{
					XMFLOAT4X4 worldMatrix;
				};

				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit InstancedMeshRendererComponent(unsigned int maxInstances, const std::wstring& meshFilePath, VertexShader* pVertexShader, PixelShader* pPixelShader, GeometryShader* pGeometryShader = nullptr, bool isUsingTransparency = false);
				virtual ~InstancedMeshRendererComponent() override;

				virtual void Initialize() override;
				virtual void Update() override;
				virtual void Render(Renderer& renderer) const override;

				PLUGIN_API void AddInstance(const std::wstring& diffuseTexturePath, const XMFLOAT3& position, const XMFLOAT3& rotation = XMFLOAT3{ 0.f, 0.f, 0.f }, const XMFLOAT3& scale = XMFLOAT3{ 1.f, 1.f, 1.f });

			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				InstancedMeshRendererComponent()
					: m_MaxInstances()
				{

				}
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				XMFLOAT4X4 GetTransposedWorldMatrix(unsigned int index) const;

				const unsigned int m_MaxInstances;

				const MeshData* m_pMeshData;
				ID3D11Buffer* m_pInstanceBuffer;

				VertexShader* m_pVertexShader;
				PixelShader* m_pPixelShader;
				GeometryShader* m_pGeometryShader;

				bool m_IsUsingTransparency;

				ID3D11SamplerState* m_pSamplerState;
				ID3D11BlendState* m_pBlendState;
				ID3D11DepthStencilState* m_pDepthStencilState;

				std::vector<TransformComponent*> m_pTransformComponents;
				std::vector<InstanceData> m_InstanceData;
			};

			COMPONENT_REGISTRATION(InstancedMeshRendererComponent);
		}
	}
}