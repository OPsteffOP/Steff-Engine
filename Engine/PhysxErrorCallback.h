#pragma once
#include <foundation\PxErrorCallback.h>

namespace SteffEngine
{
	namespace Physics
	{
		class PhysxErrorCallback : public physx::PxErrorCallback
		{
		public:
			PhysxErrorCallback() = default;
			virtual ~PhysxErrorCallback() override = default;

			virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
		};
	}
}