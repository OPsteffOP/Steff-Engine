#pragma once
#include "Component.h"

namespace SteffEngine
{
	namespace Core
	{
		class MeshData;

		class VertexShader;
		class PixelShader;
		class GeometryShader;

		namespace Components
		{
			class MeshRendererComponent : public Component
			{
				COMPONENT_DECLARATION();

			public:
				PLUGIN_API explicit MeshRendererComponent(MeshData* pMeshData, VertexShader* pVertexShader, PixelShader* pPixelShader, GeometryShader* pGeometryShader = nullptr, bool isUsingTransparency = false);
				PLUGIN_API explicit MeshRendererComponent(const std::wstring& meshFilePath, VertexShader* pVertexShader, PixelShader* pPixelShader, GeometryShader* pGeometryShader = nullptr, bool isUsingTransparency = false);
				virtual ~MeshRendererComponent() override;

				virtual void Initialize() override;
				virtual void VariableChanged(const std::string_view& variableName) override;

				virtual void Update() override;
				virtual void Render(Renderer& renderer) const override;

				PLUGIN_API void SetHighlightColor(const XMFLOAT4& highlightColor);

				bool RayHit(const Raycasting::Ray& ray, Raycasting::HitRecord& hitRecord) const;

				PLUGIN_API std::pair<XMFLOAT3, XMFLOAT3> GetCollisionBox() const;

				PLUGIN_API inline VertexShader* GetVertexShader() const { return m_pVertexShader; };
				PLUGIN_API inline GeometryShader* GetGeometryShader() const { return m_pGeometryShader; };
				PLUGIN_API inline PixelShader* GetPixelShader() const { return m_pPixelShader; };

			private:
				/* DEFAULT CONSTRUCTOR FOR EDITOR */
				MeshRendererComponent();
				/* DEFAULT CONSTRUCTOR FOR EDITOR */

				void UpdateShaders();

				EDITOR_READWRITE String m_MeshFilePath;
				EDITOR_READWRITE ShaderID m_Shader;

				MeshData* m_pMeshData;
				VertexShader* m_pVertexShader;
				GeometryShader* m_pGeometryShader;
				PixelShader* m_pPixelShader;

				EDITOR_READWRITE bool m_IsUsingTransparency;

				ID3D11SamplerState* m_pSamplerState;
				ID3D11BlendState* m_pBlendState;
				ID3D11DepthStencilState* m_pDepthStencilState;
			};

			COMPONENT_REGISTRATION(MeshRendererComponent);
		}
	}
}