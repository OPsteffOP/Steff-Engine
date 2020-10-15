#pragma once
namespace SteffEngine
{
	namespace Core
	{
		class DeviceManager
		{
		public:
			static void Initialize();
			static void Destroy();

			static ID3D11Device* m_pDevice;
			static ID3D11DeviceContext* m_pDeviceContext;
		};
	}
}