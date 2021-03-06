#include "allocator.h"
#include <string.h>

namespace snuffbox
{
	namespace engine
	{
		//-----------------------------------------------------------------------------------------------
		Allocator::Allocator(size_t max_memory_) :
			max_memory_(max_memory_),
			allocated_(0),
			num_allocations_(0)
		{

		}

		//-----------------------------------------------------------------------------------------------
		void* Allocator::Malloc(size_t size, size_t align)
		{
			std::lock_guard<std::recursive_mutex> lock(allocator_mutex_);

			size_t s = size + sizeof(Header);

			assert(allocated_ + size <= max_memory_);

			void* ptr = Allocate(s, align);

			assert(ptr != nullptr);

			Header h;
			h.size = size;

			memcpy(ptr, &h, sizeof(Header));
			ptr = reinterpret_cast<unsigned char*>(ptr) + sizeof(Header);

			allocated_ += size;
			++num_allocations_;

			return ptr;
		}

		//-----------------------------------------------------------------------------------------------
		void Allocator::Free(void* ptr)
		{
			std::lock_guard<std::recursive_mutex> lock(allocator_mutex_);

			assert(ptr != nullptr);
			assert(num_allocations_ > 0);

			ptr = reinterpret_cast<unsigned char*>(ptr) - sizeof(Header);
			Header* h = reinterpret_cast<Header*>(ptr);
			size_t size = h->size;
			assert(allocated_ >= size);

			Deallocate(ptr);

			allocated_ -= size;
			--num_allocations_;
		}

		//-----------------------------------------------------------------------------------------------
		Allocator::~Allocator()
		{
			assert(num_allocations_ == 0);
			assert(allocated_ == 0);
		}
	}
}
