#include "malloc_allocator.h"

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		MallocAllocator::MallocAllocator(size_t size) :
			Allocator(size)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void* MallocAllocator::Allocate(size_t size, size_t align)
		{
			return malloc(size);
		}

		//-----------------------------------------------------------------------------------------------
		void MallocAllocator::Deallocate(void* ptr)
		{
			free(ptr);
		}
	}
}