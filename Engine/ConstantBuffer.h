#pragma once
#include "DeviceManager.h"
#include "Logging.h"

namespace SteffEngine
{
	namespace Core
	{
#pragma region HEADER_PART
		template<typename T>
		class ConstantBuffer
		{
		public:
			ConstantBuffer(const ConstantBuffer& constantBuffer) = delete;
			ConstantBuffer(ConstantBuffer&& constantBuffer) noexcept;

			virtual ~ConstantBuffer();

			ConstantBuffer& operator=(const ConstantBuffer& constantBuffer) = delete;
			ConstantBuffer& operator=(ConstantBuffer&& constantBuffer) noexcept;

			void ApplyChanges();
			void ApplyChangesVector();

			void BindVS(uint32_t bindSlot) const;
			void BindPS(uint32_t bindSlot) const;

			T data;

		protected:
			virtual void UpdateChanges(T* pData, size_t size) const = 0;

			explicit ConstantBuffer(D3D11_USAGE usage, unsigned int cpuAccess);
			explicit ConstantBuffer(const T& data, D3D11_USAGE usage, unsigned int cpuAccess);

			ID3D11Buffer* m_pConstantBuffer;
		};
#pragma endregion

#pragma region CPP_PART
		template<typename T>
		ConstantBuffer<T>::ConstantBuffer(D3D11_USAGE usage, unsigned int cpuAccess)
			: ConstantBuffer({}, usage, cpuAccess)
		{

		}

		template<typename T>
		ConstantBuffer<T>::ConstantBuffer(const T& data, D3D11_USAGE usage, unsigned int cpuAccess)
			: data(data)
			, m_pConstantBuffer(nullptr)
		{
			D3D11_BUFFER_DESC bufferDescriptor{};
			bufferDescriptor.ByteWidth = sizeof(T);
			bufferDescriptor.Usage = usage;
			bufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDescriptor.CPUAccessFlags = cpuAccess;
			bufferDescriptor.MiscFlags = 0;
			bufferDescriptor.StructureByteStride = sizeof(T);

			D3D11_SUBRESOURCE_DATA initData{};
			initData.pSysMem = &data;

			HRESULT result{ DeviceManager::m_pDevice->CreateBuffer(&bufferDescriptor, &initData, &m_pConstantBuffer) };
			if (FAILED(result))
			{
				SteffEngine::Tools::Logging::Log(SteffEngine::Tools::LogType::ERROR, "Failed to create constant buffer");
			}
		}

		template<typename T>
		ConstantBuffer<T>::ConstantBuffer(ConstantBuffer&& constantBuffer) noexcept
			: data(constantBuffer.data)
		{
			m_pConstantBuffer = constantBuffer.m_pConstantBuffer;
			constantBuffer.m_pConstantBuffer = nullptr;
		}

		template<typename T>
		ConstantBuffer<T>::~ConstantBuffer()
		{
			SAFE_DELETE_DIRECTX(m_pConstantBuffer);
		}

		template<typename T>
		ConstantBuffer<T>& ConstantBuffer<T>::operator=(ConstantBuffer&& constantBuffer) noexcept
		{
			data = constantBuffer.data;

			SAFE_DELETE_DIRECTX(m_pConstantBuffer);

			m_pConstantBuffer = constantBuffer.m_pConstantBuffer;
			constantBuffer.m_pConstantBuffer = nullptr;

			return *this;
		}

		template<typename T>
		void ConstantBuffer<T>::ApplyChanges()
		{
			UpdateChanges(&data, sizeof(T));
		}

		template<typename T>
		void ConstantBuffer<T>::ApplyChangesVector()
		{
			UpdateChanges(data.data(), data.size() * sizeof(T));
		}

		template<typename T>
		void ConstantBuffer<T>::BindVS(uint32_t bindSlot) const
		{
			DeviceManager::m_pDeviceContext->VSSetConstantBuffers(bindSlot, 1, &m_pConstantBuffer);
		}

		template<typename T>
		void ConstantBuffer<T>::BindPS(uint32_t bindSlot) const
		{
			DeviceManager::m_pDeviceContext->PSSetConstantBuffers(bindSlot, 1, &m_pConstantBuffer);
		}
#pragma endregion
	}
}