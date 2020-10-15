#pragma once
#include <foundation\PxAllocatorCallback.h>

namespace SteffEngine
{
	namespace Physics
	{
		// https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/API.html#memory-management
		class PhysxAllocator : public physx::PxAllocatorCallback
		{
		public:
			PhysxAllocator() = default;
			virtual ~PhysxAllocator() override = default;

			virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override;
			virtual void deallocate(void* ptr) override;
		};
	}
}