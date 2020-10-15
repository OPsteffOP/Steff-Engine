#pragma once
#include "ConstantBuffer.h"

namespace SteffEngine
{
	namespace Core
	{
#pragma region HEADER_PART
		template<typename T>
		class ConstantBufferDynamic : public ConstantBuffer<T>
		{
		public:
			ConstantBufferDynamic();
			explicit ConstantBufferDynamic(const T& data);

		private:
			virtual void UpdateChanges(T* pData, size_t size) const override;
		};
#pragma endregion

#pragma region CPP_PART
		template<typename T>
		ConstantBufferDynamic<T>::ConstantBufferDynamic()
			: ConstantBuffer<T>(D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE)
		{

		}

		template<typename T>
		ConstantBufferDynamic<T>::ConstantBufferDynamic(const T& data)
			: ConstantBuffer<T>(data, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE)
		{
			
		}

		template<typename T>
		void ConstantBufferDynamic<T>::UpdateChanges(T* pData, size_t size) const
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HRESULT result = DeviceManager::m_pDeviceContext->Map(this->m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (SUCCEEDED(result))
			{
				CopyMemory(mappedResource.pData, pData, size);
				DeviceManager::m_pDeviceContext->Unmap(this->m_pConstantBuffer, 0);
			}
		}
#pragma endregion
	}
}