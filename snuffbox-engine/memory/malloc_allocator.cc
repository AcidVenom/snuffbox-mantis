#include "malloc_allocator.h"

namespace snuffbox
{
	//-----------------------------------------------------------------------------------------------
	MallocAllocator::MallocAllocator(const size_t& size) :
		Allocator(size)
	{

	}

	//-----------------------------------------------------------------------------------------------
	void* MallocAllocator::Allocate(const size_t& size, const size_t& align)
	{
		return malloc(size);
	}

	//-----------------------------------------------------------------------------------------------
	void MallocAllocator::Deallocate(void* ptr)
	{
		free(ptr);
	}
}