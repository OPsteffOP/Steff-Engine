#include "pch.h"
#include "DeviceManager.h"

using namespace SteffEngine::Core;

ID3D11Device* DeviceManager::m_pDevice{ nullptr };
ID3D11DeviceContext* DeviceManager::m_pDeviceContext{ nullptr };

void DeviceManager::Initialize()
{
	uint32_t createDeviceFlags{};
#if defined DEBUG || defined _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);
	if (FAILED(result))
		Logging::Log(LogType::ERROR, "Failed to create d3d11 device!");
}

void DeviceManager::Destroy()
{
	SAFE_DELETE_DIRECTX(m_pDeviceContext);
	SAFE_DELETE_DIRECTX(m_pDevice);
}