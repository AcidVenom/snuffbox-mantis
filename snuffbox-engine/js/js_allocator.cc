#include "js_allocator.h"


namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		JSAllocator::JSAllocator(engine::Allocator& allocator) :
			allocator_(allocator)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void* JSAllocator::Allocate(size_t length) 
		{
			void* data = AllocateUninitialized(length);
			return data == nullptr ? data : memset(data, 0, length);
		}

		//-----------------------------------------------------------------------------------------------
		void* JSAllocator::AllocateUninitialized(size_t length)
		{ 
			return allocator_.Malloc(length);
		}

		//-----------------------------------------------------------------------------------------------
		void JSAllocator::Free(void* data, size_t length) 
		{ 
			allocator_.Free(data);
		}
	}
}