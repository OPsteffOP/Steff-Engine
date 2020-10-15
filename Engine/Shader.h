#pragma once
namespace SteffEngine
{
	namespace Core
	{
		class Shader
		{
		public:
			PLUGIN_API explicit Shader() = default;
			Shader(const Shader& shader) = delete;
			Shader(Shader&& shader) = delete;

			virtual ~Shader() = default;

			Shader& operator=(const Shader& shader) = delete;
			Shader& operator=(Shader&& shader) = delete;

			PLUGIN_API virtual void RootBind() const = 0;

		protected:
			virtual void Bind() const = 0;

			ID3DBlob* m_pShaderBlob;
		};
	}
}