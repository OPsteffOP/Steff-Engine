#pragma once
#include "ConstantBuffer.h"

namespace SteffEngine
{
	namespace Core
	{
#pragma region HEADER_PART
		template<typename T>
		class ConstantBufferStatic : public ConstantBuffer<T>
		{
		public:
			ConstantBufferStatic();
			explicit ConstantBufferStatic(const T& data);

		protected:
			virtual void UpdateChanges(T* pData, size_t size) const override;
		};
#pragma endregion

#pragma region CPP_PART
		template<typename T>
		ConstantBufferStatic<T>::ConstantBufferStatic()
			: ConstantBuffer<T>(D3D11_USAGE_DEFAULT, NULL)
		{

		}

		template<typename T>
		ConstantBufferStatic<T>::ConstantBufferStatic(const T& data)
			: ConstantBuffer<T>(data, D3D11_USAGE_DEFAULT, NULL)
		{

		}

		template<typename T>
		void ConstantBufferStatic<T>::UpdateChanges(T* pData, size_t size) const
		{
			DeviceManager::m_pDeviceContext->UpdateSubresource(this->m_pConstantBuffer, 0, nullptr, pData, 0, 0);
		}
#pragma endregion
	}
}