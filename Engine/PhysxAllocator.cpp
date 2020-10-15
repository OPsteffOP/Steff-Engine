#include "pch.h"
#include "PhysxAllocator.h"

using namespace SteffEngine::Physics;

void* PhysxAllocator::allocate(size_t size, const char* typeName, const char* filename, int line)
{
	return _aligned_malloc(size, 16);
}

void PhysxAllocator::deallocate(void* ptr)
{
	_aligned_free(ptr);
}